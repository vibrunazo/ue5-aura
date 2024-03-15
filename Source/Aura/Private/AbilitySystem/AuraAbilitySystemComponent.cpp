// vib


#include "AbilitySystem/AuraAbilitySystemComponent.h"

/**
 * The ability actor info was set, because InitAbilityActorInfo was already called.
 * So now it's safe to bind delegates.
 */
void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);

	// GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Yellow, FString("EffectApplied: %s"), *TagContainer.ToString());
	// GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Yellow, FString::Printf(TEXT("EffectApplied: %s"), *TagContainer.ToString()));
	// UE_LOG(LogTemp, Warning, TEXT("EffectApplied: %s"), *TagContainer.ToString());
	// for (FGameplayTag Tag : TagContainer)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Yellow, FString::Printf(TEXT("Tag: %s"), *Tag.ToString()));
	// 	UE_LOG(LogTemp, Warning, TEXT("Tag: %s"), *Tag.ToString());
	// }
}
