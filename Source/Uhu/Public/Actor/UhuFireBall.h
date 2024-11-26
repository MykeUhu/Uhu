// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "Actor/UhuProjectile.h"
#include "UhuFireBall.generated.h"

/**
 * 
 */
UCLASS()
class UHU_API AUhuFireBall : public AUhuProjectile
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;

	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnHit() override;
};