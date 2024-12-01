#include "AbilitySystem/Abilities/Movement/MovementSpeedAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"

UMovementSpeedAbility::UMovementSpeedAbility()
    : GameplayTags(FUhuGameplayTags::Get())
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    SpeedLevels.Add(GameplayTags.Movement_Speed_0, 0.f);
    SpeedLevels.Add(GameplayTags.Movement_Speed_1, 100.f);
    SpeedLevels.Add(GameplayTags.Movement_Speed_2, 200.f);
    SpeedLevels.Add(GameplayTags.Movement_Speed_3, 300.f);
    SpeedLevels.Add(GameplayTags.Movement_Speed_4, 400.f);
    SpeedLevels.Add(GameplayTags.Movement_Speed_5, 500.f);
    SpeedLevels.Add(GameplayTags.Movement_Speed_6, 600.f);
    SpeedLevels.Add(GameplayTags.Movement_Speed_7, 700.f);
    SpeedLevels.Add(GameplayTags.Movement_Speed_8, 800.f);
    SpeedLevels.Add(GameplayTags.Movement_Speed_9, 900.f);
    SpeedLevels.Add(GameplayTags.Movement_Speed_10, 1000.f);

    StaminaRegenRates.Add(GameplayTags.Movement_Speed_0, 8.f);
    StaminaRegenRates.Add(GameplayTags.Movement_Speed_1, 4.f);
    StaminaRegenRates.Add(GameplayTags.Movement_Speed_2, 2.f);
    StaminaRegenRates.Add(GameplayTags.Movement_Speed_3, 1.f);
    StaminaRegenRates.Add(GameplayTags.Movement_Speed_4, 0.f);
    StaminaRegenRates.Add(GameplayTags.Movement_Speed_5, -1.f);
    StaminaRegenRates.Add(GameplayTags.Movement_Speed_6, -2.f);
    StaminaRegenRates.Add(GameplayTags.Movement_Speed_7, -3.f);
    StaminaRegenRates.Add(GameplayTags.Movement_Speed_8, -4.f);
    StaminaRegenRates.Add(GameplayTags.Movement_Speed_9, -5.f);
    StaminaRegenRates.Add(GameplayTags.Movement_Speed_10, -6.f);
}

void UMovementSpeedAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    UE_LOG(LogTemp, Log, TEXT("UMovementSpeedAbility::ActivateAbility called"));

    if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid ActorInfo or AvatarActor"));
        return;
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("Character cast failed"));
        return;
    }

    UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
    if (!MovementComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("CharacterMovementComponent not found"));
        return;
    }

    UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
    if (!ASC)
    {
        UE_LOG(LogTemp, Warning, TEXT("AbilitySystemComponent not found"));
        return;
    }

    int32 Level = FMath::Clamp(GetAbilityLevel(), 0, 10);
    FGameplayTag SpeedTag = FGameplayTag::RequestGameplayTag(FName(*FString::Printf(TEXT("Movement.Speed.%d"), Level)));

    if (!SpeedTag.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("GameplayTag Movement.Speed.%d could not be registered!"), Level);
        return;
    }

    float NewSpeed = SpeedLevels.FindRef(SpeedTag);
    float StaminaRegenRate = StaminaRegenRates.FindRef(SpeedTag);

    MovementComponent->MaxWalkSpeed = NewSpeed;

    if (StaminaEffect)
    {
        FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
        EffectContext.AddSourceObject(this);

        FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(StaminaEffect, GetAbilityLevel(), EffectContext);
        if (SpecHandle.IsValid())
        {
            FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
            Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Stamina")), StaminaRegenRate);

            FActiveGameplayEffectHandle ActiveGEHandle = ASC->ApplyGameplayEffectSpecToSelf(*Spec);
            UE_LOG(LogTemp, Log, TEXT("Gameplay effect applied with Stamina Regen Rate: %f"), StaminaRegenRate);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Movement Speed set to: %f, Stamina Regen Rate: %f"), NewSpeed, StaminaRegenRate);
}
