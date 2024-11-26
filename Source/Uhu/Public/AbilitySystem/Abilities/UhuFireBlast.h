// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/UhuDamageGameplayAbility.h"
#include "UhuFireBlast.generated.h"

class AUhuFireBall;
/**
 * 
 */
UCLASS()
class UHU_API UUhuFireBlast : public UUhuDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<AUhuFireBall*> SpawnFireBalls();
protected:

	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	int32 NumFireBalls = 12;

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AUhuFireBall> FireBallClass;
};