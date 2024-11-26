// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UhuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UHU_API UUhuUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};