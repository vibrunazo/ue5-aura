// vib

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NativeGameplayTags.h"

namespace AuraTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Test_TestTag)
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Strength)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Agility)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Intelligence)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Resilience)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Primary_Vigor)

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_Armor)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ArmorPenetration)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_BlockChance)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitChance)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitDamage)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_CriticalHitResistance)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_HealthRegeneration)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_ManaRegeneration)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxHealth)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Secondary_MaxMana)

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Arcane)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Fire)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Frost)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Lightning)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attributes_Resistance_Physical)

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4)

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Arcane)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Fire)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Frost)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Lightning)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical)

	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Attack)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Abilities_Summon)
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_LeftHand)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_RightHand)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Tail)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(CombatSocket_Weapon)
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_1)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_2)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_3)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Montage_Attack_4)
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effects_HitReact)

}
/**
 * AuraGameplayTags
 *
 * Singleton containing native GameplayTags.
 */
struct FAuraGameplayTags
{
	static FAuraGameplayTags& Get()	{ return GameplayTags; }
	static void InitializeNativeGameplayTags();

	/**
	 * Key: DamageType
	 * Value: Resisted DamageType
	 * Example: <Damage_Fire, Attributes_Resistance_Fire>
	 */
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;

private:
	static FAuraGameplayTags GameplayTags;
};