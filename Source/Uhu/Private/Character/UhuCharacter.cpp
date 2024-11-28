// Copyright by MykeUhu

#include "Character/UhuCharacter.h"
#include "AbilitySystemComponent.h"
#include "CineCameraComponent.h"
#include "UhuGameplayTags.h"
#include "AbilitySystem/UhuAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/UhuPlayerController.h"
#include "Player/UhuPlayerState.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/HUD/UhuHUD.h"
#include "Net/UnrealNetwork.h"

AUhuCharacter::AUhuCharacter()
{
    // Set this character to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Camera boom setup
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
    CameraBoom->SetupAttachment(GetRootComponent());
    CameraBoom->TargetArmLength = 300.0f;
    CameraBoom->bUsePawnControlRotation = true;

    // Follow camera setup
    FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>("FollowCameraComponent");
    FollowCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCameraComponent->bUsePawnControlRotation = false;

    // AFK Camera setup
    AFKCameraComponent = CreateDefaultSubobject<UCineCameraComponent>("AFKCameraComponent");
    AFKCameraComponent->SetupAttachment(GetRootComponent());
    AFKCameraComponent->SetActive(false);

    // Level-up effect setup
    LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
    LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
    LevelUpNiagaraComponent->bAutoActivate = false;

    // Character movement setup
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->bSnapToPlaneAtStart = true;

    // Controller rotation setup
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationYaw = false;

    // Character class setup
    CharacterClass = ECharacterClass::Elementalist;
}

void AUhuCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // Init ability actor info for the Server
    InitAbilityActorInfo();
    AddCharacterAbilities();
}

void AUhuCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    // Init ability actor info for the Client
    InitAbilityActorInfo();
}

void AUhuCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    
    if (AUhuPlayerController* PC = Cast<AUhuPlayerController>(GetController()))
    {
        float MouseX, MouseY;
        PC->GetInputMouseDelta(MouseX, MouseY);

        // Reset AFK timer on mouse movement
        if (MouseX != 0.0f || MouseY != 0.0f)
        {
            PC->ResetAFKTimer();
        }
    }
}

void AUhuCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    if (AUhuPlayerController* PC = Cast<AUhuPlayerController>(GetController()))
    {
        PC->OnAFKStatusChanged.AddDynamic(this, &AUhuCharacter::HandleAFKStatusChange);
    }
}

void AUhuCharacter::AddToXP_Implementation(int32 InXP)
{
    AUhuPlayerState* UhuPlayerState = GetPlayerState<AUhuPlayerState>();
    check(UhuPlayerState);
    UhuPlayerState->AddToXP(InXP);
}

void AUhuCharacter::LevelUp_Implementation()
{
    MulticastLevelUpParticles();
}

void AUhuCharacter::MulticastLevelUpParticles_Implementation() const
{
    if (IsValid(LevelUpNiagaraComponent))
    {
        const FVector CameraLocation = FollowCameraComponent->GetComponentLocation();
        const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
        const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
        LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
        LevelUpNiagaraComponent->Activate(true);
    }
}

int32 AUhuCharacter::GetXP_Implementation() const
{
    const AUhuPlayerState* UhuPlayerState = GetPlayerState<AUhuPlayerState>();
    check(UhuPlayerState);
    return UhuPlayerState->GetXP();
}

int32 AUhuCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
    const AUhuPlayerState* UhuPlayerState = GetPlayerState<AUhuPlayerState>();
    check(UhuPlayerState);
    return UhuPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AUhuCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
    const AUhuPlayerState* UhuPlayerState = GetPlayerState<AUhuPlayerState>();
    check(UhuPlayerState);
    return UhuPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 AUhuCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
    const AUhuPlayerState* UhuPlayerState = GetPlayerState<AUhuPlayerState>();
    check(UhuPlayerState);
    return UhuPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void AUhuCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
    AUhuPlayerState* UhuPlayerState = GetPlayerState<AUhuPlayerState>();
    check(UhuPlayerState);
    UhuPlayerState->AddToLevel(InPlayerLevel);

    if (UUhuAbilitySystemComponent* UhuASC = Cast<UUhuAbilitySystemComponent>(GetAbilitySystemComponent()))
    {
        UhuASC->UpdateAbilityStatuses(UhuPlayerState->GetPlayerLevel());
    }
}

void AUhuCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
    AUhuPlayerState* UhuPlayerState = GetPlayerState<AUhuPlayerState>();
    check(UhuPlayerState);
    UhuPlayerState->AddToAttributePoints(InAttributePoints);
}

void AUhuCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
    AUhuPlayerState* UhuPlayerState = GetPlayerState<AUhuPlayerState>();
    check(UhuPlayerState);
    UhuPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 AUhuCharacter::GetAttributePoints_Implementation() const
{
    AUhuPlayerState* UhuPlayerState = GetPlayerState<AUhuPlayerState>();
    check(UhuPlayerState);
    return UhuPlayerState->GetAttributePoints();
}

int32 AUhuCharacter::GetSpellPoints_Implementation() const
{
    AUhuPlayerState* UhuPlayerState = GetPlayerState<AUhuPlayerState>();
    check(UhuPlayerState);
    return UhuPlayerState->GetSpellPoints();
}

void AUhuCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
    if (AUhuPlayerController* UhuPlayerController = Cast<AUhuPlayerController>(GetController()))
    {
        UhuPlayerController->ShowMagicCircle(DecalMaterial);
        UhuPlayerController->bShowMouseCursor = false;
    }
}

void AUhuCharacter::HideMagicCircle_Implementation()
{
    if (AUhuPlayerController* UhuPlayerController = Cast<AUhuPlayerController>(GetController()))
    {
        UhuPlayerController->HideMagicCircle();
        UhuPlayerController->bShowMouseCursor = true;
    }
}

int32 AUhuCharacter::GetPlayerLevel_Implementation()
{
    const AUhuPlayerState* UhuPlayerState = GetPlayerState<AUhuPlayerState>();
    check(UhuPlayerState);
    return UhuPlayerState->GetPlayerLevel();
}

void AUhuCharacter::OnRep_Stunned()
{
    if (UUhuAbilitySystemComponent* UhuASC = Cast<UUhuAbilitySystemComponent>(AbilitySystemComponent))
    {
        const FUhuGameplayTags& GameplayTags = FUhuGameplayTags::Get();
        FGameplayTagContainer BlockedTags;
        BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
        BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
        BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
        BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
        if (bIsStunned)
        {
            UhuASC->AddLooseGameplayTags(BlockedTags);
            StunDebuffComponent->Activate();
        }
        else
        {
            UhuASC->RemoveLooseGameplayTags(BlockedTags);
            StunDebuffComponent->Deactivate();
        }
    }
}

void AUhuCharacter::OnRep_Burned()
{
    if (bIsBurned)
    {
        BurnDebuffComponent->Activate();
    }
    else
    {
        BurnDebuffComponent->Deactivate();
    }
}

void AUhuCharacter::HandleAFKStatusChange(bool bIsAFK)
{
    if (bIsAFK)
    {
        // Switch to cinematic camera
        FollowCameraComponent->SetActive(false);
        AFKCameraComponent->SetActive(true);
        
        // Disable player input
        if (AUhuPlayerController* PC = Cast<AUhuPlayerController>(GetController()))
        {
            PC->SetIgnoreLookInput(true);
            PC->SetIgnoreMoveInput(true);
        }
    }
    else
    {
        // Switch back to gameplay camera
        AFKCameraComponent->SetActive(false);
        FollowCameraComponent->SetActive(true);
        
        // Re-enable player input
        if (AUhuPlayerController* PC = Cast<AUhuPlayerController>(GetController()))
        {
            PC->SetIgnoreLookInput(false);
            PC->SetIgnoreMoveInput(false);
        }
    }
}

void AUhuCharacter::InitAbilityActorInfo()
{
    AUhuPlayerState* UhuPlayerState = GetPlayerState<AUhuPlayerState>();
    check(UhuPlayerState);
    UhuPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(UhuPlayerState, this);
    Cast<UUhuAbilitySystemComponent>(UhuPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
    AbilitySystemComponent = UhuPlayerState->GetAbilitySystemComponent();
    AttributeSet = UhuPlayerState->GetAttributeSet();
    OnAscRegistered.Broadcast(AbilitySystemComponent);
    AbilitySystemComponent->RegisterGameplayTagEvent(FUhuGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AUhuCharacter::StunTagChanged);

    if (AUhuPlayerController* UhuPlayerController = Cast<AUhuPlayerController>(GetController()))
    {
        if (AUhuHUD* UhuHUD = Cast<AUhuHUD>(UhuPlayerController->GetHUD()))
        {
            UhuHUD->InitOverlay(UhuPlayerController, UhuPlayerState, AbilitySystemComponent, AttributeSet);
        }
    }
    InitializeDefaultAttributes();
}


