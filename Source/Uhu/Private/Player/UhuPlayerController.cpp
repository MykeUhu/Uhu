// Copyright by MykeUhu

#include "Player/UhuPlayerController.h"
#include "Character/UhuCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "UhuGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/UhuAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Uhu/Uhu.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "Input/UhuInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"

AUhuPlayerController::AUhuPlayerController()
{
    bReplicates = true;
    Spline = CreateDefaultSubobject<USplineComponent>("Spline");
    LastActivityTime = 0.0f;
}

void AUhuPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    CursorTrace();
    AutoRun();
    UpdateMagicCircleLocation();
    HandlePlayerInput();
}

void AUhuPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
    if (!IsValid(MagicCircle))
    {
        MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
        if (DecalMaterial && MagicCircle)
        {
            MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
        }
    }
}

void AUhuPlayerController::HideMagicCircle()
{
    if (IsValid(MagicCircle))
    {
        MagicCircle->Destroy();
        MagicCircle = nullptr;
    }
}

void AUhuPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
    if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
    {
        UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
        DamageText->RegisterComponent();
        DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
    }
}

void AUhuPlayerController::AutoRun()
{
    if (!bAutoRunning) return;
    if (APawn* ControlledPawn = GetPawn())
    {
        const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
        const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
        ControlledPawn->AddMovementInput(Direction);

        const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
        if (DistanceToDestination <= AutoRunAcceptanceRadius)
        {
            bAutoRunning = false;
        }
    }
}

void AUhuPlayerController::UpdateMagicCircleLocation()
{
    if (IsValid(MagicCircle))
    {
        MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
    }
}

void AUhuPlayerController::CursorTrace()
{
    if (GetASC() && GetASC()->HasMatchingGameplayTag(FUhuGameplayTags::Get().Player_Block_CursorTrace))
    {
        if (LastActor) LastActor->UnHighlightActor();
        if (ThisActor) ThisActor->UnHighlightActor();
        LastActor = nullptr;
        ThisActor = nullptr;
        return;
    }
    const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
    GetHitResultUnderCursor(TraceChannel, false, CursorHit);
    if (!CursorHit.bBlockingHit) return;

    LastActor = ThisActor;
    ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

    if (LastActor != ThisActor)
    {
        if (LastActor) LastActor->UnHighlightActor();
        if (ThisActor) ThisActor->HighlightActor();
    }
}

void AUhuPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (GetASC() && GetASC()->HasMatchingGameplayTag(FUhuGameplayTags::Get().Player_Block_InputPressed))
    {
        return;
    }
    if (InputTag.MatchesTagExact(FUhuGameplayTags::Get().InputTag_LMB))
    {
        bTargeting = ThisActor ? true : false;
        bAutoRunning = false;
    }
    if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void AUhuPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (GetASC() && GetASC()->HasMatchingGameplayTag(FUhuGameplayTags::Get().Player_Block_InputReleased))
    {
        return;
    }
    if (!InputTag.MatchesTagExact(FUhuGameplayTags::Get().InputTag_LMB))
    {
        if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
        return;
    }

    if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
    
    if (!bTargeting && !bShiftKeyDown)
    {
        const APawn* ControlledPawn = GetPawn();
        if (FollowTime <= ShortPressThreshold && ControlledPawn)
        {
            if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
            {
                Spline->ClearSplinePoints();
                for (const FVector& PointLoc : NavPath->PathPoints)
                {
                    Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
                }
                if (NavPath->PathPoints.Num() > 0)
                {
                    CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
                    bAutoRunning = true;
                }
            }
            if (GetASC() && !GetASC()->HasMatchingGameplayTag(FUhuGameplayTags::Get().Player_Block_InputPressed))
            {
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
            }
        }
        FollowTime = 0.f;
        bTargeting = false;
    }
}

void AUhuPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (GetASC() && GetASC()->HasMatchingGameplayTag(FUhuGameplayTags::Get().Player_Block_InputHeld))
    {
        return;
    }
    if (!InputTag.MatchesTagExact(FUhuGameplayTags::Get().InputTag_LMB))
    {
        if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
        return;
    }

    if (bTargeting || bShiftKeyDown)
    {
        if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
    }
    else
    {
        FollowTime += GetWorld()->GetDeltaSeconds();
        if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;

        if (APawn* ControlledPawn = GetPawn())
        {
            const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
            ControlledPawn->AddMovementInput(WorldDirection);
        }
    }
}

UUhuAbilitySystemComponent* AUhuPlayerController::GetASC()
{
    if (UhuAbilitySystemComponent == nullptr)
    {
        UhuAbilitySystemComponent = Cast<UUhuAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
    }
    return UhuAbilitySystemComponent;
}

void AUhuPlayerController::BeginPlay()
{
    Super::BeginPlay();
    check(UhuContext);

    // Set up the Enhanced Input system
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        Subsystem->AddMappingContext(UhuContext, 0);
    }

    // Start the AFK check timer
    GetWorld()->GetTimerManager().SetTimer(AFKTimer, this, &AUhuPlayerController::CheckForAFK, 1.0f, true);
}

void AUhuPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Cast to our custom input component
    UUhuInputComponent* UhuInputComponent = CastChecked<UUhuInputComponent>(InputComponent);

    // Bind actions to functions
    UhuInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUhuPlayerController::Move);
    UhuInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUhuPlayerController::Look);
    UhuInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AUhuPlayerController::ShiftPressed);
    UhuInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AUhuPlayerController::ShiftReleased);

    // Bind ability actions
    UhuInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AUhuPlayerController::Look(const FInputActionValue& LookActionValue)
{
    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        const FVector2D LookAxisVector = LookActionValue.Get<FVector2D>();
        
        // Apply yaw rotation
        ControlledPawn->AddControllerYawInput(LookAxisVector.X);

        // Apply pitch rotation (with optional inversion)
        float PitchInput = bInvertYAxis ? LookAxisVector.Y : -LookAxisVector.Y;
        ControlledPawn->AddControllerPitchInput(PitchInput);

        // Limit pitch angle
        PlayerCameraManager->ViewPitchMin = -70.0f;
        PlayerCameraManager->ViewPitchMax = 0.f;
    }
}

void AUhuPlayerController::Move(const FInputActionValue& InputActionValue)
{
    // Reset AFK timer on movement
    ResetAFKTimer();

    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    // Get forward and right vectors
    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        // Apply movement
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}

void AUhuPlayerController::ResetAFKTimer()
{
    LastActivityTime = GetWorld()->GetTimeSeconds();
    if (bIsAFK)
    {
        ExitAFKMode();
    }
}

void AUhuPlayerController::CheckForAFK()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastActivityTime >= AFKThreshold && !bIsAFK)
    {
        EnterAFKMode();
    }
}

void AUhuPlayerController::EnterAFKMode()
{
    if (!bIsAFK)
    {
        bIsAFK = true;
        OnAFKStatusChanged.Broadcast(true);
        UE_LOG(LogTemp, Log, TEXT("UhuPlayerController: Entered AFK Mode"));
        
        AUhuCharacter* UhuCharacter = Cast<AUhuCharacter>(GetPawn());
        if (UhuCharacter)
        {
            UhuCharacter->HandleAFKStatusChange(true);
        }
    }
}

void AUhuPlayerController::ExitAFKMode()
{
    if (bIsAFK)
    {
        bIsAFK = false;
        OnAFKStatusChanged.Broadcast(false);
        UE_LOG(LogTemp, Log, TEXT("UhuPlayerController: Exited AFK Mode"));
        
        AUhuCharacter* UhuCharacter = Cast<AUhuCharacter>(GetPawn());
        if (UhuCharacter)
        {
            UhuCharacter->HandleAFKStatusChange(false);
        }
    }
}

void AUhuPlayerController::HandlePlayerInput()
{
    // Check for any input that should reset the AFK timer
    if (IsInputKeyDown(EKeys::AnyKey) || GetInputAnalogKeyState(EKeys::MouseX) != 0.f || GetInputAnalogKeyState(EKeys::MouseY) != 0.f)
    {
        // Exclude F12 key
        if (!IsInputKeyDown(EKeys::F12))
        {
            ResetAFKTimer();
        }
    }
}