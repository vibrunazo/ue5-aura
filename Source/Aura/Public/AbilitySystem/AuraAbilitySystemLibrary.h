// vib

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (WorldContext="WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (WorldContext="WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	/**
	 * Applies a GameplayEffect to a given Target AbilitySystemComponent. Will create the Spec and Handle from the Class
	 * then apply it using a default empty EffectContext.
	 * @param EffectClass EffectClass to Apply
	 * @param ASC Target Ability System the Effect will be applied to
	 * @param Level Effect level to apply, defaults to 1
	 */
	static void ApplyEffectFromClass(TSubclassOf<UGameplayEffect> EffectClass, UAbilitySystemComponent* ASC,
	                                 float Level = 1.f);

	/**
	 * Applies the Primary Attributes (Strength, Agi, Int etc) from a given CharacterClass (Warrior, Mage etc) of given
	 * Level to a target AbilitySystemComponent. Retrieves the appropriate GameplayEffect from the GameMode's
	 * ClassDefaultsInfo DataAsset then applies it to the ASC.
	 * @param WorldContextObject Any in-game Object. Required to get the GameMode
	 * @param CharacterClass The CharacterClass to retrieve PrimaryAttributes from
	 * @param ASC Target AbilitySystemComponent the Effect will be applied to
	 * @param Level Character Level, defaults to 1
	 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext="WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, UAbilitySystemComponent* ASC, float Level = 1.f);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext="WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	/**
	 * Returns the CharacterClassDefaultInfo Asset, stored in the GameMode. And stores common classes related to each
	 * Character Class. Can only be called on the server since the GameMode only exists on the server. Will return a
	 * nullptr if called on a client.
	 * @param WorldContextObject Any in-game Object. Required to get the GameMode
	 * @return The CharacterClassDefaultInfo Asset
	 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext="WorldContextObject"))
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
	/**
	 * Searches for live players within a given sphere radius around the Origin.
	 * @param WorldContextObject Required to call Static functions
	 * @param OutLivePlayers Array of Actors that will be filled up with the live players
	 * @param ActorsToIgnore Array of Actors that will be ignored
	 * @param Radius Radius of the Sphere we will search in
	 * @param Origin Origin of the sphere of search
	 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutLivePlayers, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& Origin);
	
	
};
