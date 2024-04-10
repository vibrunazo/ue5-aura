// vib


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceArcane)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceFire)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceFrost)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistanceLightning)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ResistancePhysical)

	/**
	 * Key: GameplayTag relative to the Attribute
	 * Value: FGameplayEffectAttributeCaptureDefinition for the captured Attribute relative to that Tag
	 * Example: Attributes_Resistance_Arcane, ResistanceArcaneDef
	 */
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, true)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, true)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, true)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false)

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistanceArcane, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistanceFire, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistanceFrost, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistanceLightning, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ResistancePhysical, Target, false)

		TagsToCaptureDefs.Add(AuraTags::Attributes_Resistance_Arcane, ResistanceArcaneDef);
		TagsToCaptureDefs.Add(AuraTags::Attributes_Resistance_Fire, ResistanceFireDef);
		TagsToCaptureDefs.Add(AuraTags::Attributes_Resistance_Frost, ResistanceFrostDef);
		TagsToCaptureDefs.Add(AuraTags::Attributes_Resistance_Lightning, ResistanceLightningDef);
		TagsToCaptureDefs.Add(AuraTags::Attributes_Resistance_Physical, ResistancePhysicalDef);}
};

static AuraDamageStatics GetDamageStatics()
{
	static AuraDamageStatics DamageStatics;
	return DamageStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().CriticalHitResistanceDef);

	RelevantAttributesToCapture.Add(GetDamageStatics().ResistanceArcaneDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ResistanceFireDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ResistanceFrostDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ResistanceLightningDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().ResistancePhysicalDef);	
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationBonusCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetrationBonus"), FString());
	// Gets the Bonus Armor Penetration the attacker gets for being higher level than the defender.
	const float SourceArmorPenetrationLevelBonus = ArmorPenetrationBonusCurve->Eval(SourceCombatInterface->GetPlayerLevel() - TargetCombatInterface->GetPlayerLevel());
	const FRealCurve* ArmorBonusCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorBonus"), FString());
	// Gets the Armor bonus the defender gets from being higher level than the attacker.
	const float TargetArmorLevelBonus = ArmorBonusCurve->Eval(TargetCombatInterface->GetPlayerLevel() - SourceCombatInterface->GetPlayerLevel());
	const FRealCurve* CriticalChanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalChanceResistance"), FString());
	// Gets the coefficient multiplied by critical chance depending on defender level. Higher level targets are harder to crit against.
	const float CriticalChanceLevelCoefficient = CriticalChanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(0.f, SourceArmorPenetration + SourceArmorPenetrationLevelBonus);
	
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Clamp(TargetCriticalHitResistance, 0.f, 100.f);
	
	float SourceCriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalChance);
	SourceCriticalChance = FMath::Clamp((SourceCriticalChance - TargetCriticalHitResistance) * CriticalChanceLevelCoefficient, 0.f, 100.f);

	float SourceCriticalDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalDamage);
	SourceCriticalDamage = FMath::Clamp(SourceCriticalDamage, 0.f, 400.f);

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max(0.f, TargetArmor + TargetArmorLevelBonus - SourceArmorPenetration);

	// Get Damage Set by Caller Magnitude
	// float Damage = Spec.GetSetByCallerMagnitude(AuraTags::Damage);
	float Damage = 0.f;
	// TODO: Use Tags in the Effect itself to determine type of damage instead of looping through all existing types.
	// FGameplayTagContainer AllDamageTags = UGameplayTagsManager::Get().RequestGameplayTagChildren(AuraTags::Damage);
	for(const TTuple<FGameplayTag, FGameplayTag>& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTypeTag = Pair.Value;
		const auto CapturedAttributeDef = GetDamageStatics().TagsToCaptureDefs[ResistanceTypeTag];
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CapturedAttributeDef, EvaluationParameters, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag);
		Damage += DamageTypeValue * (100.f - Resistance) / 100.f;
	}

	Damage -= TargetArmor;
	Damage = FMath::Max(0.f, Damage);
	
	// Capture Block chance on the target, and determine if there was a block
	float BlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().BlockChanceDef, EvaluationParameters, BlockChance);
	// If Block, halve Damage
	const float HitRoll = FMath::FRandRange(0.f, 100.f);
	const bool bBlocked = HitRoll <= BlockChance;
	if (bBlocked) Damage *= 0.5f;

	// Roll for crit chance
	const bool bCrit = !bBlocked && (HitRoll <= BlockChance + SourceCriticalChance);
	if (bCrit) Damage *= 1.f + SourceCriticalDamage / 100.f;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCrit);
	
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
	
}
