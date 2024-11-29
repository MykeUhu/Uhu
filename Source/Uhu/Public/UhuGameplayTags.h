// Copyright by MykeUhu

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * UhuGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FUhuGameplayTags
{
public:
    static const FUhuGameplayTags& Get() { return GameplayTags;}
    static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;

	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	
	
	FGameplayTag Attributes_Secondary_MaxMana;
	
	FGameplayTag Attributes_Meta_IncomingXP;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_Shift;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;
	

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;

	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;

	FGameplayTag Abilities_None;
	
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	
	FGameplayTag Abilities_HitReact;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Fire_FireBlast;	
	FGameplayTag Abilities_Lightning_Electrocute;
	FGameplayTag Abilities_Arcane_ArcaneShards;


	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;

	FGameplayTag Cooldown_Fire_FireBolt;

	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;

	FGameplayTag Effects_HitReact;

	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;

	FGameplayTag GameplayCue_FireBlast;

	/*
	* Uhu Attributes
	*/
	// Vital Attributes
	FGameplayTag Attributes_Vital_Health; // Gesundheit: Die aktuelle Menge an Gesundheitspunkten.
	FGameplayTag Attributes_Vital_Hunger; // Hunger: Die aktuelle Menge an Hungerpunkten.
	FGameplayTag Attributes_Vital_Thirst; // Durst: Die aktuelle Menge an Durstpunkten.
	FGameplayTag Attributes_Vital_Stamina; // Ausdauer: Die aktuelle Menge an Ausdauerpunkten.
	FGameplayTag Attributes_Vital_Temperature; // Temperatur: Die aktuelle Körpertemperatur des Charakters.


	// Primary Attributes
	FGameplayTag Attributes_Primary_Endurance; // Ausdauer: Beeinflusst die maximale Gesundheit und Ausdauer.
	FGameplayTag Attributes_Primary_Strength; // Stärke: Beeinflusst die körperliche Kraft und Tragfähigkeit.
	FGameplayTag Attributes_Primary_Agility; // Geschicklichkeit: Beeinflusst die Beweglichkeit und Geschwindigkeit.
	FGameplayTag Attributes_Primary_Cleverness; // Intelligenz: Beeinflusst die Wahrnehmung und Kommunikationsfähigkeiten.
	FGameplayTag Attributes_Primary_Cleanliness; // Sauberkeit: Beeinflusst die Gesundheitsregeneration und Widerstandsfähigkeit gegen Krankheiten.

	// Secondary Attributes
	FGameplayTag Attributes_Secondary_MaxHealth; // Maximale Gesundheit: Die maximale Menge an Gesundheitspunkten.
	FGameplayTag Attributes_Secondary_MaxHunger; // Maximaler Hunger: Die maximale Menge an Hungerpunkten.
	FGameplayTag Attributes_Secondary_MaxThirst; // Maximaler Durst: Die maximale Menge an Durstpunkten.
	FGameplayTag Attributes_Secondary_MaxStamina; // Maximale Ausdauer: Die maximale Menge an Ausdauerpunkten.
	FGameplayTag Attributes_Secondary_Perception; // Wahrnehmung: Beeinflusst die Fähigkeit, Umgebungsdetails zu erkennen.
	FGameplayTag Attributes_Secondary_Communication; // Kommunikation: Beeinflusst die Fähigkeit, mit anderen zu interagieren.

	// Uhu Resistance
	FGameplayTag Attributes_Secondary_FoodPoisonResistance; // Lebensmittelvergiftungsresistenz: Widerstandsfähigkeit gegen Lebensmittelvergiftungen.
	FGameplayTag Attributes_Secondary_TemperatureResistance; // Temperaturresistenz: Widerstandsfähigkeit gegen extreme Temperaturen.

	// Uhu Buff
	FGameplayTag Attributes_Secondary_HealthRegeneration; // Gesundheitsregeneration: Die Rate, mit der Gesundheitspunkte regeneriert werden.
	FGameplayTag Attributes_Secondary_HungerRegeneration; // Hungerregeneration: Die Rate, mit der Hungerpunkte regeneriert werden.
	FGameplayTag Attributes_Secondary_ThirstRegeneration; // Durstregeneration: Die Rate, mit der Durstpunkte regeneriert werden.
	FGameplayTag Attributes_Secondary_StaminaRegeneration; // Ausdauerregeneration: Die Rate, mit der Ausdauerpunkte regeneriert werden.

	// Uhu Debuff
	FGameplayTag Attributes_Secondary_HealthDegeneration; // Gesundheitsdegeneration: Die Rate, mit der Gesundheitspunkte verloren gehen.
	FGameplayTag Attributes_Secondary_HungerDegeneration; // Hungerdegeneration: Die Rate, mit der Hungerpunkte verloren gehen.
	FGameplayTag Attributes_Secondary_ThirstDegeneration; // Durstdegeneration: Die Rate, mit der Durstpunkte verloren gehen.
	FGameplayTag Attributes_Secondary_StaminaDegeneration; // Ausdauerdegeneration: Die Rate, mit der Ausdauerpunkte verloren gehen.

	// Debuff
	FGameplayTag Debuff_Starvation; // Verhungern: Zustand, in dem der Charakter aufgrund von Nahrungsmangel leidet.
	FGameplayTag Debuff_Dehydration; // Dehydrierung: Zustand, in dem der Charakter aufgrund von Wassermangel leidet.
	FGameplayTag Debuff_Hypothermia; // Unterkühlung: Zustand, in dem der Charakter aufgrund von Kälte leidet.
	FGameplayTag Debuff_Heatstroke; // Hitzschlag: Zustand, in dem der Charakter aufgrund von Hitze leidet.
	FGameplayTag Debuff_FoodPoisoning; // Lebensmittelvergiftung: Zustand, in dem der Charakter aufgrund von verdorbenem Essen leidet.



	// Damage
	FGameplayTag Damage_Starvation;
	FGameplayTag Damage_Dehydration;
	FGameplayTag Damage_Hypothermia;
	FGameplayTag Damage_Heatstroke;
	FGameplayTag Damage_FoodPoisoning;


	
private:
    static FUhuGameplayTags GameplayTags;
};