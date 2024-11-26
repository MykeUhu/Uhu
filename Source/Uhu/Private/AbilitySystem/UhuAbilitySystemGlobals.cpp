// Copyright by MykeUhu


#include "AbilitySystem/UhuAbilitySystemGlobals.h"

#include "UhuAbilityTypes.h"

FGameplayEffectContext* UUhuAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FUhuGameplayEffectContext();
}