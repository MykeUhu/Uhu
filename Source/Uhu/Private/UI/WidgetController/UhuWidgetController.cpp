// Copyright by MykeUhu


#include "UI/WidgetController/UhuWidgetController.h"

#include "Player/UhuPlayerController.h"
#include "Player/UhuPlayerState.h"
#include "AbilitySystem/UhuAbilitySystemComponent.h"
#include "AbilitySystem/UhuAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void UUhuWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UUhuWidgetController::BroadcastInitialValues()
{
	
}

void UUhuWidgetController::BindCallbacksToDependencies()
{
	
}

void UUhuWidgetController::BroadcastAbilityInfo()
{
	if (!GetUhuASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FUhuAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(UhuAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = UhuAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = UhuAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetUhuASC()->ForEachAbility(BroadcastDelegate);
}

AUhuPlayerController* UUhuWidgetController::GetUhuPC()
{
	if (UhuPlayerController == nullptr)
	{
		UhuPlayerController = Cast<AUhuPlayerController>(PlayerController);
	}
	return UhuPlayerController;
}

AUhuPlayerState* UUhuWidgetController::GetUhuPS()
{
	if (UhuPlayerState == nullptr)
	{
		UhuPlayerState = Cast<AUhuPlayerState>(PlayerState);
	}
	return UhuPlayerState;
}

UUhuAbilitySystemComponent* UUhuWidgetController::GetUhuASC()
{
	if (UhuAbilitySystemComponent == nullptr)
	{
		UhuAbilitySystemComponent = Cast<UUhuAbilitySystemComponent>(AbilitySystemComponent);
	}
	return UhuAbilitySystemComponent;
}

UUhuAttributeSet* UUhuWidgetController::GetUhuAS()
{
	if (UhuAttributeSet == nullptr)
	{
		UhuAttributeSet = Cast<UUhuAttributeSet>(AttributeSet);
	}
	return UhuAttributeSet;
}