// Copyright by MykeUhu

#include "Player/UhuPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "UhuGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/UhuAbilitySystemComponent.h"
#include "Character/UhuCharacter.h"
#include "Input/UhuInputComponent.h"

AUhuPlayerController::AUhuPlayerController()
{
    bReplicates = true;
}

void AUhuPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
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

    bShowMouseCursor = false;
    SetInputMode(FInputModeGameOnly());
}

void AUhuPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UUhuInputComponent* UhuInputComponent = CastChecked<UUhuInputComponent>(InputComponent);
    UhuInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUhuPlayerController::Move);
    UhuInputComponent->BindAction(SpeedAction, ETriggerEvent::Triggered, this, &AUhuPlayerController::Speed);
    UhuInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AUhuPlayerController::Move(const FInputActionValue& InputActionValue)
{
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

void AUhuPlayerController::Speed(const FInputActionValue& InputActionValue)
{
    const float ScrollValue = InputActionValue.Get<float>();
    UE_LOG(LogTemp, Log, TEXT("Scroll Value: %f"), ScrollValue);

    // Adjust the speed level based on scroll direction
    CurrentSpeedLevel = FMath::Clamp(CurrentSpeedLevel + (ScrollValue > 0 ? 1 : -1), 0, MaxSpeedLevel);
    UE_LOG(LogTemp, Log, TEXT("New Speed Level: %d"), CurrentSpeedLevel);

    if (AUhuCharacter* UhuCharacter = Cast<AUhuCharacter>(GetCharacter()))
    {
        UAbilitySystemComponent* ASC = UhuCharacter->GetAbilitySystemComponent();
        if (ASC)
        {
            // Find the ability spec for the movement speed ability
            FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromClass(MovementSpeedAbility);
            if (AbilitySpec)
            {
                // Update the ability level
                AbilitySpec->Level = CurrentSpeedLevel;
                ASC->MarkAbilitySpecDirty(*AbilitySpec);

                // Activate or update the ability
                if (ASC->TryActivateAbility(AbilitySpec->Handle))
                {
                    UE_LOG(LogTemp, Log, TEXT("Movement Speed ability activated at level: %d"), CurrentSpeedLevel);
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to activate Movement Speed ability at level: %d"), CurrentSpeedLevel);
                }
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Movement Speed ability spec not found!"));
            }
        }
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
    // Placeholder für spätere Implementierung
}
