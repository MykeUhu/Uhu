// Copyright by MykeUhu


#include "Character/UhuEnemy.h"

#include "AbilitySystem/UhuAbilitySystemComponent.h"
#include "AbilitySystem/UhuAbilitySystemLibrary.h"
#include "AbilitySystem/UhuAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "Uhu/Uhu.h"
#include "UI/Widget/UhuUserWidget.h"
#include "UhuGameplayTags.h"
#include "AI/UhuAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AUhuEnemy::AUhuEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UUhuAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UUhuAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	
	BaseWalkSpeed = 250.f;
}

void AUhuEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	UhuAIController = Cast<AUhuAIController>(NewController);
	UhuAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	UhuAIController->RunBehaviorTree(BehaviorTree);
	UhuAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	UhuAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void AUhuEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AUhuEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AUhuEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}

void AUhuEnemy::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(LifeSpan);
	if (UhuAIController) UhuAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	
	Super::Die(DeathImpulse);
}

void AUhuEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AUhuEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AUhuEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UUhuAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);	
	}

	
	if (UUhuUserWidget* UhuUserWidget = Cast<UUhuUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		UhuUserWidget->SetWidgetController(this);
	}
	
	if (const UUhuAttributeSet* UhuAS = Cast<UUhuAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UhuAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UhuAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		
		AbilitySystemComponent->RegisterGameplayTagEvent(FUhuGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AUhuEnemy::HitReactTagChanged
		);

		OnHealthChanged.Broadcast(UhuAS->GetHealth());
		OnMaxHealthChanged.Broadcast(UhuAS->GetMaxHealth());
	}
	
}

void AUhuEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (UhuAIController && UhuAIController->GetBlackboardComponent())
	{
		UhuAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AUhuEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UUhuAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(FUhuGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AUhuEnemy::StunTagChanged);


	if (HasAuthority())
	{
		InitializeDefaultAttributes();		
	}
	OnAscRegistered.Broadcast(AbilitySystemComponent);
}

void AUhuEnemy::InitializeDefaultAttributes() const
{
	UUhuAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void AUhuEnemy::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);
	
	if (UhuAIController && UhuAIController->GetBlackboardComponent())
	{
		UhuAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stunned"), bIsStunned);
	}
}