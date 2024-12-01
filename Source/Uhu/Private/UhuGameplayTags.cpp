// Copyright by MykeUhu


#include "UhuGameplayTags.h"
#include "GameplayTagsManager.h"

FUhuGameplayTags FUhuGameplayTags::GameplayTags;

void FUhuGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Uhu Attributes
	 */
	// Vital Attributes
    GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Vital.Health"),
        FString("Health Vital Attribute")
		);
	
    GameplayTags.Attributes_Vital_Hunger = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Vital.Hunger"),
        FString("Hunger Vital Attribute")
		);
	
    GameplayTags.Attributes_Vital_Thirst = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Vital.Thirst"),
        FString("Thirst Vital Attribute")
		);
	
    GameplayTags.Attributes_Vital_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Vital.Stamina"),
        FString("Stamina Vital Attribute")
		);
	
	
    GameplayTags.Attributes_Vital_Temperature = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Vital.Temperature"),
        FString("Temperature Vital Attribute")
		);

    // Primary Attributes
	GameplayTags.Attributes_Primary_Endurance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Primary.Endurance"),
	FString("Endurance Primary Attribute")
		);
	
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Primary.Strength"),
	FString("Strength Primary Attribute")
		);

	GameplayTags.Attributes_Primary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Primary.Agility"),
	FString("Agility Primary Attribute")
		);
	
    GameplayTags.Attributes_Primary_Cleverness = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Cleverness"),
        FString("Cleverness Primary Attribute")
		);

    GameplayTags.Attributes_Primary_Cleanliness = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Primary.Cleanliness"),
        FString("Cleanliness Primary Attribute")
		);

    // Secondary Attributes
    GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.MaxHealth"),
        FString("Max Health Secondary Attribute")
		);
	
    GameplayTags.Attributes_Secondary_MaxHunger = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.MaxHunger"),
        FString("Max Hunger Secondary Attribute")
		);
	
	GameplayTags.Attributes_Secondary_HungerDegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HungerDegeneration"),
		FString("Hunger Degeneration Secondary Attribute")
		);
	
    GameplayTags.Attributes_Secondary_MaxThirst = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.MaxThirst"),
        FString("Max Thirst Secondary Attribute")
		);
	
	GameplayTags.Attributes_Secondary_ThirstDegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ThirstDegeneration"),
		FString("Thirst Degeneration Secondary Attribute")
		);
	
    GameplayTags.Attributes_Secondary_MaxStamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.MaxStamina"),
        FString("Max Stamina Secondary Attribute")
		);
	
    GameplayTags.Attributes_Secondary_Perception = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.Perception"),
        FString("Perception Secondary Attribute")
		);
	
    GameplayTags.Attributes_Secondary_Communication = UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.Communication"),
        FString("Communication Secondary Attribute")
		);

	// Uhu Resistance
	GameplayTags.Attributes_Secondary_FoodPoisonResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.FoodPoisonResistance"),
		FString("Food Poison Resistance")
		);
	
	GameplayTags.Attributes_Secondary_TemperatureResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.TemperatureResistance"),
		FString("Temperature Resistance")
		);


	
	// Uhu Buff
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Buff.HealthRegeneration"),
		FString("Health Regeneration Buff")
		);

	GameplayTags.Attributes_Secondary_HungerRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Buff.HungerRegeneration"),
		FString("Hunger Regeneration Buff")
		);
	
	GameplayTags.Attributes_Secondary_ThirstRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Buff.ThirstRegeneration"),
		FString("Thirst Regeneration Buff")
		);
	
	GameplayTags.Attributes_Secondary_StaminaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Buff.StaminaRegeneration"),
		FString("Stamina Regeneration Buff")
		);
	
	// Uhu Debuff
	GameplayTags.Attributes_Secondary_HealthDegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.HealthDegeneration"),
		FString("Stamina Degeneration Debuff")
		);
	
		GameplayTags.Attributes_Secondary_HungerDegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
    	FName("Debuff.HungerDegeneration"),
    	FString("Stamina Degeneration Debuff")
    	);

	GameplayTags.Attributes_Secondary_ThirstDegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.ThirstDegeneration"),
		FString("Stamina Degeneration Debuff")
		);
	
	GameplayTags.Attributes_Secondary_StaminaDegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.StaminaDegeneration"),
		FString("Stamina Degeneration Debuff")
		);

	// Uhu Movement
	GameplayTags.Movement_Speed_0 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Movement.Speed.0"),
			FString("Speed 0 - Don't Move")
		);

		GameplayTags.Movement_Speed_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Movement.Speed.1"),
			FString("Speed 100 - Walking")
		);

		GameplayTags.Movement_Speed_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Movement.Speed.2"),
			FString("Speed 200 - Jogging")
		);

		GameplayTags.Movement_Speed_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Movement.Speed.3"),
			FString("Speed 300 - Running")
		);

		GameplayTags.Movement_Speed_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Movement.Speed.4"),
			FString("Speed 400 - Sprinting")
		);

		GameplayTags.Movement_Speed_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Movement.Speed.5"),
			FString("Speed 500 - Fast Sprinting")
		);

		GameplayTags.Movement_Speed_6 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Movement.Speed.6"),
			FString("Speed 600 - Very Fast Sprinting")
		);

		GameplayTags.Movement_Speed_7 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Movement.Speed.7"),
			FString("Speed 700 - Super Sprinting")
		);

		GameplayTags.Movement_Speed_8 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Movement.Speed.8"),
			FString("Speed 800 - Ultra Sprinting")
		);

		GameplayTags.Movement_Speed_9 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Movement.Speed.9"),
			FString("Speed 900 - Hyper Sprinting")
		);

		GameplayTags.Movement_Speed_10 = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Movement.Speed.10"),
			FString("Speed 1000 - Maximum Speed")
		);

		GameplayTags.Data_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(
				FName("Data.Stamina"),
				FString("Regen Degen Data for Movement")
			);

	// --

	/*
	 * Primary Attributes
	 */
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases magical damage")
		);

	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration")
		);

	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health")
		);

	/*
	 * Secondary Attributes
	 */
	
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves Block Chance")
		);

	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ArmorPenetration"),
		FString("Ignores Percentage of enemy Armor, increases Critical Hit Chance")
		);

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming damage in half")
		);

	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage plus critical hit bonus")
		);

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a critical hit is scored")
		);

	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduces Critical Hit Chance of attacking enemies")
		);

	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of Mana regenerated every 1 second")
		);

	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of Mana obtainable")
		);

	/*
	 * Input Tags
	 */
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
		);

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button")
		);

	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input Tag for 1 key")
		);

	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input Tag for 2 key")
		);

	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input Tag for 3 key")
		);

	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input Tag for 4 key")
		);

	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.1"),
		FString("Input Tag Passive Ability 1")
		);

	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Passive.2"),
		FString("Input Tag Passive Ability 2")
		);

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
		);

	/*
	 * Damage Types
	 */

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Type")
		);
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage Type")
		);
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Arcane Damage Type")
		);
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type")
		);

	/*
	 * Resistances
	 */

	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"),
		FString("Resistance to Arcane damage")
		);
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"),
		FString("Resistance to Fire damage")
		);
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"),
		FString("Resistance to Lightning damage")
		);
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"),
		FString("Resistance to Physical damage")
		);

	/*
	 * Debuffs
	 */

	GameplayTags.Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Arcane"),
		FString("Debuff for Arcane damage")
		);
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Burn"),
		FString("Debuff for Fire damage")
		);
	GameplayTags.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Physical"),
		FString("Debuff for Physical damage")
		);
	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Stun"),
		FString("Debuff for Lightning damage")
		);

	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Chance"),
		FString("Debuff Chance")
		);
	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Damage"),
		FString("Debuff Damage")
		);
	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Duration"),
		FString("Debuff Duration")
		);
	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Frequency"),
		FString("Debuff Frequency")
		);

	/*
	 * Meta Attributes
	 */

	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP Meta Attribute")
		);

	
	
	/*
	 * Map of Damage Types to Resistances
	 */
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);

	/*
	 * Map of Damage Types to Debuffs
	 */
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);

	/*
	 * Effects
	 */

	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag granted when Hit Reacting")
		);

	/*
	 * Abilities
	 */

	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.None"),
		FString("No Ability - like the nullptr for Ability Tags")
		);

	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Ability Tag")
		);

	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"),
		FString("Summon Ability Tag")
		);

	/*
	 * Offensive Spells
	 */

	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBolt"),
		FString("FireBolt Ability Tag")
		);

	GameplayTags.Abilities_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBlast"),
		FString("FireBlast Ability Tag")
		);

	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Lightning.Electrocute"),
		FString("Electrocute Ability Tag")
		);

	GameplayTags.Abilities_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Arcane.ArcaneShards"),
		FString("Arcane Shards Ability Tag")
		);

	/*
	 * Passive Spells
	 */
	
	GameplayTags.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.LifeSiphon"),
			FString("Life Siphon")
			);
	GameplayTags.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.ManaSiphon"),
			FString("Mana Siphon")
			);
	GameplayTags.Abilities_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(
			FName("Abilities.Passive.HaloOfProtection"),
			FString("Halo Of Protection")
			);
	

	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.HitReact"),
		FString("Hit React Ability")
		);

	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"),
		FString("Eligible Status")
		);

	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"),
		FString("Equipped Status")
		);

	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Locked"),
		FString("Locked Status")
		);

	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Status.Unlocked"),
		FString("Unlocked Status")
		);

	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.None"),
		FString("Type None")
		);

	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"),
		FString("Type Offensive")
		);

	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Type.Passive"),
		FString("Type Passive")
		);

	/*
	* Cooldown
	*/
	
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBolt"),
		FString("FireBolt Cooldown Tag")
		);

	/*
	 * Combat Sockets
	 */

	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"),
		FString("Weapon")
		);

	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightHand"),
		FString("Right Hand")
		);
	
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"),
		FString("Left Hand")
		);

	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Tail"),
		FString("Tail")
		);

	/*
	 * Montage Tags
	 */

	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"),
		FString("Attack 1")
		);

	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"),
		FString("Attack 2")
		);

	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"),
		FString("Attack 3")
		);

	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"),
		FString("Attack 4")
		);

	/*
	 * Player Tags
	 */

	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.CursorTrace"),
		FString("Block tracing under the cursor")
		);

	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputHeld"),
		FString("Block Input Held callback for input")
		);

	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputPressed"),
		FString("Block Input Pressed callback for input")
		);

	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Player.Block.InputReleased"),
		FString("Block Input Released callback for input")
		);

	/*
	 * GameplayCues
	 */

	GameplayTags.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.FireBlast"),
		FString("FireBlast GameplayCue Tag")
		);

	
	// Damage
	GameplayTags.Damage_FoodPoisoning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.FoodPoisoning"),
		FString("Damage caused by food poisoning")
	);

	GameplayTags.Damage_Hypothermia = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Hypothermia"),
		FString("Damage caused by hypothermia")
	);

	GameplayTags.Damage_Heatstroke = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Heatstroke"),
		FString("Damage caused by heatstroke")
	);

	GameplayTags.Damage_Starvation = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Starvation"),
		FString("Damage caused by starvation")
	);

	GameplayTags.Damage_Dehydration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Dehydration"),
		FString("Damage caused by dehydration")
	);
	
	// Debuffs
	GameplayTags.Debuff_Starvation = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Starvation"),
		FString("Debuff applied when starving")
	);

	GameplayTags.Debuff_Hypothermia = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Hypothermia"),
		FString("Debuff applied when experiencing hypothermia")
	);

	GameplayTags.Debuff_Heatstroke = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Heatstroke"),
		FString("Debuff applied when experiencing heatstroke")
	);

	GameplayTags.Debuff_FoodPoisoning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.FoodPoisoning"),
		FString("Debuff applied when experiencing food poisoning")
	);

	GameplayTags.Debuff_Dehydration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Debuff.Dehydration"),
		FString("Debuff applied when experiencing dehydration")
	);
	


	// Map of Damage Types to Resistances
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_FoodPoisoning, GameplayTags.Attributes_Secondary_FoodPoisonResistance);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Hypothermia, GameplayTags.Attributes_Secondary_TemperatureResistance);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Heatstroke, GameplayTags.Attributes_Secondary_TemperatureResistance);
	
	// Map of Damage Types to Debuffs
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Starvation, GameplayTags.Debuff_Starvation);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Hypothermia, GameplayTags.Debuff_Hypothermia);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Heatstroke, GameplayTags.Debuff_Heatstroke);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_FoodPoisoning, GameplayTags.Debuff_FoodPoisoning);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Dehydration, GameplayTags.Debuff_Dehydration);
	
}