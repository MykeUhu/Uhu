// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "UhuGameplayTags.h"
#include "MovementSpeedAbility.generated.h"

UCLASS()
class UHU_API UMovementSpeedAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMovementSpeedAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	TMap<FGameplayTag, float> SpeedLevels;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina")
	TMap<FGameplayTag, float> StaminaRegenRates;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina")
	TSubclassOf<UGameplayEffect> StaminaEffect;

private:
	// const FUhuGameplayTags& GameplayTags;
	const FUhuGameplayTags GameplayTags;
};