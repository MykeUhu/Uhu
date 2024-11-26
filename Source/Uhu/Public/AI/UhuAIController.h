// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UhuAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class UHU_API AUhuAIController : public AAIController
{
	GENERATED_BODY()
public:
	AUhuAIController();
protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};