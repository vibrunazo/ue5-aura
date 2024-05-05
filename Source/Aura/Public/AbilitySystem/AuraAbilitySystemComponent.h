// vib

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraAbilitySystemComponent;
class UAuraGameplayAbility;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags */);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UAuraAbilitySystemComponent*);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoSet();

	/**
	 * Delegate called when an effect is applied to the owning actor.
	 * @param FGameplayTagContainer The container with the effects spec that were applied
	 */
	FEffectAssetTags EffectAssetTags;
	/**
	 * Delegate called when the ASC has given all startup Abilities to the controlled Pawn.
	 * Bound by the Widget Controller so widgets can know when to update abilities info the UI.
	 * @param UAuraAbilitySystemComponent* The ASC that has given all abilities
	 */
	FAbilitiesGiven AbilitiesGivenDelegate;

	void AddCharacterAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities);
	// Set after all startup abilities were given to controlled Pawn, just before the AbilitiesGivenDelegate is broadcast.
	// Used by the Widget Controller to determine whether it bound to the AbilitiesGivenDelegate too late
	// and need to manually check for abilities.
	bool bStartupAbilitiesGiven = false;

	void AbilityInputHeld(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);

protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	
	
};
