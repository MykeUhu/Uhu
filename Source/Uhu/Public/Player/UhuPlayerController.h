// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "UhuPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UUhuInputConfig;
class UUhuAbilitySystemComponent;
class UGameplayAbility;

UCLASS()
class UHU_API AUhuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AUhuPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	TSubclassOf<UGameplayAbility> MovementSpeedAbility;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	int32 MaxSpeedLevel = 10;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> UhuContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;
    
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> SpeedAction;

	void Move(const FInputActionValue& InputActionValue);
	void Speed(const FInputActionValue& InputActionValue);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UUhuInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UUhuAbilitySystemComponent> UhuAbilitySystemComponent;

	UUhuAbilitySystemComponent* GetASC();

	int32 CurrentSpeedLevel = 4; // Start at default walking speed
};