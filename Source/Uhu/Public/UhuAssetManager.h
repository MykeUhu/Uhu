// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "UhuAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class UHU_API UUhuAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:

	static UUhuAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};