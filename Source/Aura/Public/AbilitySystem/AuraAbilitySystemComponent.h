// vib

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraAbilitySystemComponent;
class UAuraGameplayAbility;
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* AssetTags */);
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);


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
	FEffectAssetTags EffectAssetTagsChangedDelegate;
	/**
	 * Delegate called when the ASC has given all startup Abilities to the controlled Pawn.
	 * Bound by the Widget Controller so widgets can know when to update abilities info the UI.
	 * @param UAuraAbilitySystemComponent* The ASC that has given all abilities
	 */
	FAbilitiesGiven AbilitiesGivenDelegate;

	/**
	 * Adds the given Abilities array to the Character's Ability System. Used only by Aura. Called only on Server for
	 * Startup Abilities.
	 * @param StartupAbilities The list of abilities to add
	 */
	void AddCharacterAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities);
	/**
	 * Adds the given Passive Abilities array to the Character's Ability System. Used only by Aura. Called only on Server
	 * for Startup Passive Abilities.
	 * @param StartupPassiveAbilities The list of abilities to add
	 */
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	// Set after all startup abilities were given to controlled Pawn, just before the AbilitiesGivenDelegate is broadcast.
	// Used by the Widget Controller to determine whether it bound to the AbilitiesGivenDelegate too late
	// and need to manually check for abilities.
	bool bStartupAbilitiesGiven = false;

	void AbilityInputHeld(const FGameplayTag& InputTag);
	void AbilityInputReleased(const FGameplayTag& InputTag);
	/**
	 * Executes a function for each ability spec in the ASC
	 * @param Delegate Function that will be called for each ability spec 
	 */
	void ExecForEachAbility(const FForEachAbility& Delegate);

	/**
	 * Returns the first Tag starting with "Abilities" in the spec's ability tags container.
	 * for example: "Abilities.FireBolt".
	 * @param Spec The ability spec
	 * @return The associated tag
	 */
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& Spec);
	/**
	 * Returns the first Tag starting with "InputTag" in the spec's Dynamic tags' container.
	 * for example: "InputTag.LMB".
	 * This is the tag that should be used to bind input to abilities.
	 * @param Spec The ability spec
	 * @return The input tag
	 */
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& Spec);

	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

protected:
	virtual void OnRep_ActivateAbilities() override;
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	
	
};
