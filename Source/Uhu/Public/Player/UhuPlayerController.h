// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "UhuPlayerController.generated.h"

class UNiagaraSystem;
class UDamageTextComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UUhuInputConfig;
class UUhuAbilitySystemComponent;
class USplineComponent;
class AMagicCircle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAFKStatusChanged, bool, bIsAFK);

/**
 * AUhuPlayerController handles player input and AFK functionality.
 */
UCLASS()
class UHU_API AUhuPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AUhuPlayerController();

    virtual void PlayerTick(float DeltaTime) override;

    UFUNCTION(Client, Reliable)
    void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

    UFUNCTION(BlueprintCallable, Category = "Magic")
    void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Magic")
    void HideMagicCircle();

    UPROPERTY(BlueprintAssignable, Category = "AFK")
    FOnAFKStatusChanged OnAFKStatusChanged;

    UFUNCTION(BlueprintPure, Category = "AFK")
    bool IsInAFKMode() const { return bIsAFK; }

    UFUNCTION(BlueprintCallable, Category = "AFK")
    void ResetAFKTimer();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Rotation")
    bool bInvertYAxis = false;

    // AFK System
    FTimerHandle AFKTimer;
    float LastActivityTime = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AFK")
    float AFKThreshold = 5.f;

    bool bIsAFK = false;

    UFUNCTION()
    void CheckForAFK();

    UFUNCTION(BlueprintCallable, Category = "AFK")
    void EnterAFKMode();

    UFUNCTION(BlueprintCallable, Category = "AFK")
    void ExitAFKMode();

private:
    // Input related properties
    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputMappingContext> UhuContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    TObjectPtr<UInputAction> ShiftAction;

    // Input handling functions
    void ShiftPressed() { bShiftKeyDown = true; };
    void ShiftReleased() { bShiftKeyDown = false; };
    bool bShiftKeyDown = false;

    void Move(const FInputActionValue& InputActionValue);
    void Look(const FInputActionValue& LookActionValue);

    // Cursor tracing
    void CursorTrace();
    IEnemyInterface* LastActor;
    IEnemyInterface* ThisActor;
    FHitResult CursorHit;

    // Ability system related functions
    void AbilityInputTagPressed(FGameplayTag InputTag);
    void AbilityInputTagReleased(FGameplayTag InputTag);
    void AbilityInputTagHeld(FGameplayTag InputTag);

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UUhuInputConfig> InputConfig;

    UPROPERTY()
    TObjectPtr<UUhuAbilitySystemComponent> UhuAbilitySystemComponent;

    UUhuAbilitySystemComponent* GetASC();

    // Auto-run related properties
    FVector CachedDestination = FVector::ZeroVector;
    float FollowTime = 0.f;
    float ShortPressThreshold = 0.5f;
    bool bAutoRunning = false;
    bool bTargeting = false;

    UPROPERTY(EditDefaultsOnly)
    float AutoRunAcceptanceRadius = 50.f;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USplineComponent> Spline;

    UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

    void AutoRun();

    // Damage text component
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

    // Magic circle properties
    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AMagicCircle> MagicCircleClass;

    UPROPERTY()
    TObjectPtr<AMagicCircle> MagicCircle;

    void UpdateMagicCircleLocation();

    // Handle player input for AFK system
    void HandlePlayerInput();
};