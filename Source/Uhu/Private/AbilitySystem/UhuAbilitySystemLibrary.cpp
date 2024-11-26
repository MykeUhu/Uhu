// Copyright by MykeUhu


#include "AbilitySystem/UhuAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "UhuAbilityTypes.h"
#include "UhuGameplayTags.h"
#include "Game/UhuGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/UhuPlayerState.h"
#include "UI/HUD/UhuHUD.h"
#include "UI/WidgetController/UhuWidgetController.h"

bool UUhuAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AUhuHUD*& OutUhuHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutUhuHUD = Cast<AUhuHUD>(PC->GetHUD());
		if (OutUhuHUD)
		{
			AUhuPlayerState* PS = PC->GetPlayerState<AUhuPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UUhuAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AUhuHUD* UhuHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, UhuHUD))
	{
		return UhuHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UUhuAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AUhuHUD* UhuHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, UhuHUD))
	{
		return UhuHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UUhuAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AUhuHUD* UhuHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, UhuHUD))
	{
		return UhuHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UUhuAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UUhuAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

int32 UUhuAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

void UUhuAbilitySystemLibrary::SetIsRadialDamageEffectParam(FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin)
{
	DamageEffectParams.bIsRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = Origin;
}

void UUhuAbilitySystemLibrary::SetKnockbackDirection(FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
	}
	else
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude;
	}
}

void UUhuAbilitySystemLibrary::SetDeathImpulseDirection(FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;
	}
	else
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;
	}
}

void UUhuAbilitySystemLibrary::SetTargetEffectParamsASC(FDamageEffectParams& DamageEffectParams,
	UAbilitySystemComponent* InASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InASC;
}

UCharacterClassInfo* UUhuAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AUhuGameModeBase* UhuGameMode = Cast<AUhuGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (UhuGameMode == nullptr) return nullptr;
	return UhuGameMode->CharacterClassInfo;
}

UAbilityInfo* UUhuAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AUhuGameModeBase* UhuGameMode = Cast<AUhuGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (UhuGameMode == nullptr) return nullptr;
	return UhuGameMode->AbilityInfo;
}

bool UUhuAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUhuGameplayEffectContext* UhuEffectContext = static_cast<const FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UhuEffectContext->IsBlockedHit();
	}
	return false;
}

bool UUhuAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUhuGameplayEffectContext* UhuEffectContext = static_cast<const FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UhuEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UUhuAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUhuGameplayEffectContext* UhuEffectContext = static_cast<const FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UhuEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UUhuAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUhuGameplayEffectContext* UhuEffectContext = static_cast<const FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UhuEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UUhuAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUhuGameplayEffectContext* UhuEffectContext = static_cast<const FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UhuEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UUhuAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUhuGameplayEffectContext* UhuEffectContext = static_cast<const FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (UhuEffectContext->GetDamageType().IsValid())
		{
			return *UhuEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UUhuAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUhuGameplayEffectContext* UhuEffectContext = static_cast<const FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UhuEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UUhuAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUhuGameplayEffectContext* UhuEffectContext = static_cast<const FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UhuEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

bool UUhuAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUhuGameplayEffectContext* UhuEffectContext = static_cast<const FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UhuEffectContext->IsCriticalHit();
	}
	return false;
}

bool UUhuAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUhuGameplayEffectContext* UhuEffectContext = static_cast<const FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UhuEffectContext->IsRadialDamage();
	}
	return false;
}

float UUhuAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUhuGameplayEffectContext* UhuEffectContext = static_cast<const FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UhuEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UUhuAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUhuGameplayEffectContext* UhuEffectContext = static_cast<const FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UhuEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UUhuAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FUhuGameplayEffectContext* UhuEffectContext = static_cast<const FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return UhuEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UUhuAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FUhuGameplayEffectContext* UhuEffectContext = static_cast<FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UhuEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UUhuAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FUhuGameplayEffectContext* UhuEffectContext = static_cast<FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UhuEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UUhuAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInSuccessfulDebuff)
{
	if (FUhuGameplayEffectContext* UhuEffectContext = static_cast<FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UhuEffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void UUhuAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FUhuGameplayEffectContext* UhuEffectContext = static_cast<FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UhuEffectContext->SetDebuffDamage(InDamage);
	}
}

void UUhuAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FUhuGameplayEffectContext* UhuEffectContext = static_cast<FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UhuEffectContext->SetDebuffDuration(InDuration);
	}
}

void UUhuAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FUhuGameplayEffectContext* UhuEffectContext = static_cast<FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UhuEffectContext->SetDebuffFrequency(InFrequency);
	}
}

void UUhuAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FUhuGameplayEffectContext* UhuEffectContext = static_cast<FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		UhuEffectContext->SetDamageType(DamageType);
	}
}

void UUhuAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InImpulse)
{
	if (FUhuGameplayEffectContext* UhuEffectContext = static_cast<FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UhuEffectContext->SetDeathImpulse(InImpulse);
	}
}

void UUhuAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InForce)
{
	if (FUhuGameplayEffectContext* UhuEffectContext = static_cast<FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UhuEffectContext->SetKnockbackForce(InForce);
	}
}

void UUhuAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsRadialDamage)
{
	if (FUhuGameplayEffectContext* UhuEffectContext = static_cast<FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UhuEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UUhuAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InInnerRadius)
{
	if (FUhuGameplayEffectContext* UhuEffectContext = static_cast<FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UhuEffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UUhuAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InOuterRadius)
{
	if (FUhuGameplayEffectContext* UhuEffectContext = static_cast<FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UhuEffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UUhuAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InOrigin)
{
	if (FUhuGameplayEffectContext* UhuEffectContext = static_cast<FUhuGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		UhuEffectContext->SetRadialDamageOrigin(InOrigin);
	}
}

void UUhuAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                           const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void UUhuAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}

	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;

	while (NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0) break;
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;
		for (AActor* PotentialTarget : ActorsToCheck)
		{
			const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = PotentialTarget;
			}
		}
		ActorsToCheck.Remove(ClosestActor);
		OutClosestTargets.AddUnique(ClosestActor);
		++NumTargetsFound;
	}
}

bool UUhuAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

FGameplayEffectContextHandle UUhuAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FUhuGameplayTags& GameplayTags = FUhuGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContexthandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContexthandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContexthandle, DamageEffectParams.KnockbackForce);

	SetIsRadialDamage(EffectContexthandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContexthandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContexthandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContexthandle, DamageEffectParams.RadialDamageOrigin);
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContexthandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContexthandle;
}

TArray<FRotator> UUhuAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}
	return Rotators;
}

TArray<FVector> UUhuAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}
	return Vectors;
}