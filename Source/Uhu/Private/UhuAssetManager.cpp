// Copyright by MykeUhu


#include "UhuAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "UhuGameplayTags.h"

UUhuAssetManager& UUhuAssetManager::Get()
{
	check(GEngine);
	
	UUhuAssetManager* UhuAssetManager = Cast<UUhuAssetManager>(GEngine->AssetManager);
	return *UhuAssetManager;
}

void UUhuAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FUhuGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData();
}