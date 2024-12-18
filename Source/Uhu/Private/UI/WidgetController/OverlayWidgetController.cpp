// Copyright by MykeUhu

#include "UI/WidgetController/OverlayWidgetController.h"

#include "UhuGameplayTags.h"
#include "AbilitySystem/UhuAbilitySystemComponent.h"
#include "AbilitySystem/UhuAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/UhuPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{

	OnHealthChanged.Broadcast(GetUhuAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetUhuAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetUhuAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetUhuAS()->GetMaxMana());
	OnThirstChanged.Broadcast(GetUhuAS()->GetThirst());
	OnMaxThirstChanged.Broadcast(GetUhuAS()->GetMaxThirst());
	OnHungerChanged.Broadcast(GetUhuAS()->GetHunger());
	OnMaxHungerChanged.Broadcast(GetUhuAS()->GetMaxHunger());
	OnStaminaChanged.Broadcast(GetUhuAS()->GetStamina());
	OnMaxThirstChanged.Broadcast(GetUhuAS()->GetMaxThirst());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetUhuPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetUhuPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetUhuAS()->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetUhuAS()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetUhuAS()->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetUhuAS()->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetUhuAS()->GetThirstAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnThirstChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetUhuAS()->GetMaxThirstAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxThirstChanged.Broadcast(Data.NewValue);
			}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetUhuAS()->GetHungerAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHungerChanged.Broadcast(Data.NewValue);
			}
		);
		
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetUhuAS()->GetMaxHungerAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHungerChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetUhuAS()->GetStaminaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnStaminaChanged.Broadcast(Data.NewValue);
			}
		);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetUhuAS()->GetMaxStaminaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxStaminaChanged.Broadcast(Data.NewValue);
			}
			);

	if (GetUhuASC())
	{
		GetUhuASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if (GetUhuASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetUhuASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}

		GetUhuASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					// For example, say that Tag = Message.HealthPotion
					// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
	
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetUhuPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out UhuPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FUhuGameplayTags& GameplayTags = FUhuGameplayTags::Get();
	
	FUhuAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FUhuAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}