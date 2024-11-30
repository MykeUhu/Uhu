// Copyright by MykeUhu

#include "Player/UhuPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "UhuGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystem/UhuAbilitySystemComponent.h"
#include "Character/UhuCharacter.h"
#include "Character/UhuCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Uhu/Uhu.h"
#include "Input/UhuInputComponent.h"

AUhuPlayerController::AUhuPlayerController()
{
    bReplicates = true;
}

void AUhuPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    // Hier können zukünftige Logiken für Tick hinzugefügt werden
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

    bShowMouseCursor = false; // Mauszeiger wird nicht benötigt
    SetInputMode(FInputModeGameOnly());
}

void AUhuPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UUhuInputComponent* UhuInputComponent = CastChecked<UUhuInputComponent>(InputComponent);
    UhuInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AUhuPlayerController::Move);
    UhuInputComponent->BindAction(SpeedAction, ETriggerEvent::Triggered, this, &AUhuPlayerController::Speed);
    UhuInputComponent->BindAction(ResetSpeedAction, ETriggerEvent::Started, this, &AUhuPlayerController::ResetSpeed);
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
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Speed method triggered"));
    }

    const float ScrollValue = InputActionValue.Get<float>();

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Scroll Value: %f"), ScrollValue));
    }

    const float MinSpeed = 200.0f;
    const float MaxSpeed = 1200.0f;

    if (AUhuCharacter* UhuCharacter = Cast<AUhuCharacter>(GetCharacter()))
    {
        float NewSpeed = FMath::Clamp(UhuCharacter->GetCharacterMovement()->MaxWalkSpeed + ScrollValue * 100.0f, MinSpeed, MaxSpeed);
        UhuCharacter->GetCharacterMovement()->MaxWalkSpeed = NewSpeed;

        // Wende den Stamina-Effekt an, wenn die Geschwindigkeit über 600 liegt
        if (NewSpeed > 600.0f)
        {
            UhuCharacter->ApplyStaminaEffect(NewSpeed);
        }

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("New Speed: %f"), NewSpeed));
        }
    }
}


void AUhuPlayerController::ResetSpeed(const FInputActionValue& InputActionValue)
{
    if (AUhuCharacter* UhuCharacter = Cast<AUhuCharacter>(GetCharacter()))
    {
        UhuCharacter->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
        if (GEngine)
            {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Speed reset to 600"));
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
