// vib

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

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
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);
	
};
