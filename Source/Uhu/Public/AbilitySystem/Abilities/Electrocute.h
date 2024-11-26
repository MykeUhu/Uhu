// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/UhuBeamSpell.h"
#include "Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class UHU_API UElectrocute : public UUhuBeamSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};