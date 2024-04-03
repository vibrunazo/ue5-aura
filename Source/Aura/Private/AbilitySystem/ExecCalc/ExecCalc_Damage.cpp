// vib


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false)
		
	}
};

static AuraDamageStatics GetDamageStatics()
{
	static AuraDamageStatics DamageStatics;
	return DamageStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(GetDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(GetDamageStatics().BlockChanceDef);
	
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Armor = 0.f;
	// Will set Armor to the value of the target Armor when hit
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().ArmorDef, EvaluationParameters, Armor);

	// Armor++;
	// const FGameplayModifierEvaluatedData EvaluatedData(GetDamageStatics().ArmorProperty, EGameplayModOp::Additive, Armor);
	
	// Get Damage Set by Caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	Damage -= Armor;
	Damage = FMath::Max(0.f, Damage);
	
	// Capture Block chance on the target, and determine if there was a block
	float BlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDamageStatics().BlockChanceDef, EvaluationParameters, BlockChance);
	// TargetASC->GetAttributeSet(UAuraAttributeSet::StaticClass());
	// BlockChance = 25.f;
	// If Block, halve Damage
	const int8 BlockRoll = FMath::RandRange(1, 100);
	const bool bBlocked = BlockRoll < BlockChance;
	if (bBlocked) Damage *= 0.5f;
	// GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Yellow,
	// 	FString::Printf(TEXT("Block? %d, Roll: %f, Chance: %f"), bBlocked, BlockRoll, BlockChance));
	// UE_LOG(LogTemp, Warning, TEXT("Block? %d, Roll: %f, Chance: %f"), bBlocked, BlockRoll, BlockChance)

	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
	
}
