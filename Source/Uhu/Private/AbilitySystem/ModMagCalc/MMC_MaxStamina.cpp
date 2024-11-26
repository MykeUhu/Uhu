// Copyright by MykeUhu


#include "AbilitySystem/ModMagCalc/MMC_MaxStamina.h"
#include "AbilitySystem/UhuAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxStamina::UMMC_MaxStamina()
{
    AgilityDef.AttributeToCapture = UUhuAttributeSet::GetAgilityAttribute();
    AgilityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    AgilityDef.bSnapshot = false;
    
    ClevernessDef.AttributeToCapture = UUhuAttributeSet::GetClevernessAttribute();
    ClevernessDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    ClevernessDef.bSnapshot = false;

    StrengthDef.AttributeToCapture = UUhuAttributeSet::GetStrengthAttribute();
    StrengthDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    StrengthDef.bSnapshot = false;

    RelevantAttributesToCapture.Add(AgilityDef);
    RelevantAttributesToCapture.Add(ClevernessDef);
    RelevantAttributesToCapture.Add(StrengthDef);

}

float UMMC_MaxStamina::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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
    
    float Cleverness = 0.f;
    GetCapturedAttributeMagnitude(ClevernessDef, Spec, EvaluationParameters, Cleverness);
    Cleverness = FMath::Max<float>(Cleverness, 0.f);

    float Strength = 0.f;
    GetCapturedAttributeMagnitude(StrengthDef, Spec, EvaluationParameters, Strength);
    Strength = FMath::Max<float>(Strength, 0.f);

    int32 PlayerLevel = 1;
    if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
    {
        PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
    }

    return 96.5f + 1.0f * Agility + 1.0f * Cleverness + 1.0f * Strength + 1.0f * PlayerLevel;
}
