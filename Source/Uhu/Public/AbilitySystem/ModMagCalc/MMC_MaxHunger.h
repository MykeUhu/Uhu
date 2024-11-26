// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHunger.generated.h"

/**
 * 
 */
UCLASS()
class UHU_API UMMC_MaxHunger : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxHunger();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition EnduranceDef;
	FGameplayEffectAttributeCaptureDefinition CleanlinessDef;
	FGameplayEffectAttributeCaptureDefinition AgilityDef;

};