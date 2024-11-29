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
    if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void AUhuPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (GetASC() && GetASC()->HasMatchingGameplayTag(FUhuGameplayTags::Get().Player_Block_InputReleased))
    {
        return;
    }
    if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
}

void AUhuPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (GetASC() && GetASC()->HasMatchingGameplayTag(FUhuGameplayTags::Get().Player_Block_InputHeld))
    {
        return;
    }
    if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
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

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        Subsystem->AddMappingContext(UhuContext, 0);
    }

    GetWorld()->GetTimerManager().SetTimer(AFKTimer, this, &AUhuPlayerController::CheckForAFK, 1.0f, true);
}

void AUhuPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UUhuInputComponent* UhuInputComponent = CastChecked<UUhuInputComponent>(InputComponent);

    UhuInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUhuPlayerController::Move);
    UhuInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AUhuPlayerController::Look);
    UhuInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AUhuPlayerController::ShiftPressed);
    UhuInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AUhuPlayerController::ShiftReleased);

    UhuInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AUhuPlayerController::ShiftPressed()
{
    if (AUhuCharacter* UhuCharacter = Cast<AUhuCharacter>(GetPawn()))
    {
        UhuCharacter->SetSprinting(true);
    }
}

void AUhuPlayerController::ShiftReleased()
{
    if (AUhuCharacter* UhuCharacter = Cast<AUhuCharacter>(GetPawn()))
    {
        UhuCharacter->SetSprinting(false);
    }
}

void AUhuPlayerController::Move(const FInputActionValue& InputActionValue)
{
    ResetAFKTimer();

    const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = GetControlRotation();
    const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}

void AUhuPlayerController::Look(const FInputActionValue& LookActionValue)
{
    if (APawn* ControlledPawn = GetPawn<APawn>())
    {
        const FVector2D LookAxisVector = LookActionValue.Get<FVector2D>();
        
        ControlledPawn->AddControllerYawInput(LookAxisVector.X);
        float PitchInput = bInvertYAxis ? LookAxisVector.Y : -LookAxisVector.Y;
        ControlledPawn->AddControllerPitchInput(PitchInput);

        if (PlayerCameraManager)
        {
            PlayerCameraManager->ViewPitchMin = -70.0f;
            PlayerCameraManager->ViewPitchMax = 0.f;
        }
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
    if (IsInputKeyDown(EKeys::AnyKey) || GetInputAnalogKeyState(EKeys::MouseX) != 0.f || GetInputAnalogKeyState(EKeys::MouseY) != 0.f)
    {
        if (!IsInputKeyDown(EKeys::F12))
        {
            ResetAFKTimer();
        }
    }
}

