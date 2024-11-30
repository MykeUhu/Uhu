// Copyright by MykeUhu


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/UhuAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	EnduranceDef.AttributeToCapture = UUhuAttributeSet::GetEnduranceAttribute();
	EnduranceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	EnduranceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(EnduranceDef);

	CleanlinessDef.AttributeToCapture = UUhuAttributeSet::GetCleanlinessAttribute();
	CleanlinessDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	CleanlinessDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(CleanlinessDef);

	CleanlinessDef.AttributeToCapture = UUhuAttributeSet::GetCleanlinessAttribute();
	CleanlinessDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	CleanlinessDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(CleanlinessDef);
	
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Strength = 0.f;
	GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluationParameters, Strength);
	Strength = FMath::Max<float>(Strength, 0.f);

	float Endurance = 0.f;
	GetCapturedAttributeMagnitude(EnduranceDef, Spec, EvaluationParameters, Endurance);
	Endurance = FMath::Max<float>(Endurance, 0.f);

	float Cleanliness = 0.f;
	GetCapturedAttributeMagnitude(CleanlinessDef, Spec, EvaluationParameters, Cleanliness);
	Endurance = FMath::Max<float>(Cleanliness, 0.f);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	return 96.5f + 1.0f * Strength + 1.0f * Endurance + 1.0f * Cleanliness + 1.0f;
}