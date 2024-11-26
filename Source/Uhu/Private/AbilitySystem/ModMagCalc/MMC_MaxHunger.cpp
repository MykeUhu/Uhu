// Copyright by MykeUhu


#include "AbilitySystem/ModMagCalc/MMC_MaxHunger.h"
#include "AbilitySystem/UhuAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHunger::UMMC_MaxHunger()
{
    EnduranceDef.AttributeToCapture = UUhuAttributeSet::GetEnduranceAttribute();
    EnduranceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    EnduranceDef.bSnapshot = false;

    CleanlinessDef.AttributeToCapture = UUhuAttributeSet::GetCleanlinessAttribute();
    CleanlinessDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    CleanlinessDef.bSnapshot = false;

    AgilityDef.AttributeToCapture = UUhuAttributeSet::GetAgilityAttribute();
    AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    AgilityDef.bSnapshot = false;

    RelevantAttributesToCapture.Add(EnduranceDef);
    RelevantAttributesToCapture.Add(CleanlinessDef);
    RelevantAttributesToCapture.Add(AgilityDef);
}

float UMMC_MaxHunger::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    // Gather tags from source and target
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    float Endurance = 0.f;
    GetCapturedAttributeMagnitude(EnduranceDef, Spec, EvaluationParameters, Endurance);
    Endurance = FMath::Max<float>(Endurance, 0.f);

    float Cleanliness = 0.f;
    GetCapturedAttributeMagnitude(CleanlinessDef, Spec, EvaluationParameters, Cleanliness);
    Cleanliness = FMath::Max<float>(Cleanliness, 0.f);

    float Agility = 0.f;
    GetCapturedAttributeMagnitude(AgilityDef, Spec, EvaluationParameters, Agility);
    Agility = FMath::Max<float>(Agility, 0.f);

    int32 PlayerLevel = 1;
    if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
    {
        PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
    }

    return 96.5f + 1.0f * Endurance + 1.0f * Cleanliness + 1.0f * Agility + 1.0f * PlayerLevel;
}
