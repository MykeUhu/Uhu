// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "Character/UhuCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "UhuCharacter.generated.h"

class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;
class UCineCameraComponent;
class UGameplayEffect;

/**
 * AUhuCharacter represents the player character in the game.
 * It handles character setup, abilities, and AFK functionality.
 */
UCLASS()
class UHU_API AUhuCharacter : public AUhuCharacterBase, public IPlayerInterface
{
    GENERATED_BODY()

public:
    AUhuCharacter();

    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;
    virtual void Tick(float DeltaSeconds) override;
    virtual void BeginPlay() override;

    /** Player Interface */
    virtual void AddToXP_Implementation(int32 InXP) override;
    virtual void LevelUp_Implementation() override;
    virtual int32 GetXP_Implementation() const override;
    virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
    virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
    virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
    virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
    virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
    virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
    virtual int32 GetAttributePoints_Implementation() const override;
    virtual int32 GetSpellPoints_Implementation() const override;
    virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial) override;
    virtual void HideMagicCircle_Implementation() override;
    /** end Player Interface */

    /** Combat Interface */
    virtual int32 GetPlayerLevel_Implementation() override;
    /** end Combat Interface */

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
    TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

    virtual void OnRep_Stunned() override;
    virtual void OnRep_Burned() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCineCameraComponent> AFKCameraComponent;

    UFUNCTION()
    void HandleAFKStatusChange(bool bIsAFK);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetSprinting(bool bNewSprinting);

    UPROPERTY(EditDefaultsOnly, Category = "Movement")
    float SprintSpeedMultiplier = 1.5f;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<UCameraComponent> FollowCameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
    TSubclassOf<UGameplayEffect> SprintEffect;

private:
    bool bIsSprinting = false;
    float BaseWalkSpeed = 600.f;
    
    virtual void InitAbilityActorInfo() override;

    UFUNCTION(NetMulticast, Reliable)
    void MulticastLevelUpParticles() const;

    void ApplySprintEffect();
    void RemoveSprintEffect();

    FActiveGameplayEffectHandle SprintEffectHandle;
};
