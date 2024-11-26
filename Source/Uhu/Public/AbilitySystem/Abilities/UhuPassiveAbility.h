// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/UhuGameplayAbility.h"
#include "UhuPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class UHU_API UUhuPassiveAbility : public UUhuGameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void ReceiveDeactivate(const FGameplayTag& AbilityTag);
};