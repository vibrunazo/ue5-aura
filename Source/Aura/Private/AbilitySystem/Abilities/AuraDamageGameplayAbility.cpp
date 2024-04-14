// vib


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1);
	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid Spec Handle on %s"), *GetName());
		return;
	}
	for (auto Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		SpecHandle.Data->SetSetByCallerMagnitude(Pair.Key, ScaledDamage);
	}
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data, UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	
}
