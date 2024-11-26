// Copyright by MykeUhu


#include "AbilitySystem/ModMagCalc/MMC_MaxThirst.h"
#include "AbilitySystem/UhuAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxThirst::UMMC_MaxThirst()
{
	AgilityDef.AttributeToCapture = UUhuAttributeSet::GetAgilityAttribute();
	AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	AgilityDef.bSnapshot = false;
	
	EnduranceDef.AttributeToCapture = UUhuAttributeSet::GetEnduranceAttribute();
	EnduranceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	EnduranceDef.bSnapshot = false;

	ClevernessDef.AttributeToCapture = UUhuAttributeSet::GetClevernessAttribute();
	ClevernessDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ClevernessDef.bSnapshot = false;


	RelevantAttributesToCapture.Add(AgilityDef);
	RelevantAttributesToCapture.Add(EnduranceDef);
	RelevantAttributesToCapture.Add(ClevernessDef);

}

float UMMC_MaxThirst::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	float Agility = 0.f;
	GetCapturedAttributeMagnitude(AgilityDef, Spec, EvaluationParameters, Agility);
	Agility = FMath::Max<float>(Agility, 0.f);
	
	float Endurance = 0.f;
	GetCapturedAttributeMagnitude(EnduranceDef, Spec, EvaluationParameters, Endurance);
	Endurance = FMath::Max<float>(Endurance, 0.f);

	float Cleverness = 0.f;
	GetCapturedAttributeMagnitude(ClevernessDef, Spec, EvaluationParameters, Cleverness);
	Cleverness = FMath::Max<float>(Cleverness, 0.f);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	return 96.5f + 1.0f * Agility + 1.0f * Endurance + 1.0f * Cleverness + 1.0f * PlayerLevel;
}