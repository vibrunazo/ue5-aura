// vib


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController();
	if (!PC) return nullptr;
	AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD());
	if (!AuraHUD) return nullptr;
	AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	UAttributeSet *AS = PS->GetAttributeSet();
	const FWidgetControllerParams WidgetControllerParams = FWidgetControllerParams(PC, PS, ASC, AS);
	return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController();
	if (!PC) return nullptr;
	AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD());
	if (!AuraHUD) return nullptr;
	AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	UAttributeSet *AS = PS->GetAttributeSet();
	const FWidgetControllerParams WidgetControllerParams = FWidgetControllerParams(PC, PS, ASC, AS);
	return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
}

void UAuraAbilitySystemLibrary::ApplyEffectFromClass(TSubclassOf<UGameplayEffect> EffectClass, UAbilitySystemComponent* ASC, float Level)
{
	const AActor* SourceActor = ASC->GetAvatarActor();
	FGameplayEffectContextHandle Context = ASC->MakeEffectContext();
	Context.AddSourceObject(SourceActor);
	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, Level, Context);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, UAbilitySystemComponent* ASC, float Level)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return;
	
	// Gets the Asset that contains the GameplayEffect class that sets the initial Primary Attributes for all Character Classes
	UCharacterClassInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	// Gets the struct that contains that GE for this given Class
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	const TSubclassOf<UGameplayEffect> PrimaryAttributesEffectClass = ClassDefaultInfo.PrimaryAttributes;
	const TSubclassOf<UGameplayEffect> SecondaryAttributesEffectClass = CharacterClassInfo->SecondaryAttributes;
	const TSubclassOf<UGameplayEffect> VitalAttributesEffectClass = CharacterClassInfo->VitalAttributes;

	ApplyEffectFromClass(PrimaryAttributesEffectClass, ASC, Level);
	ApplyEffectFromClass(SecondaryAttributesEffectClass, ASC, Level);
	ApplyEffectFromClass(VitalAttributesEffectClass, ASC, Level);
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return;
	// Gets the Asset that contains the initial Abilities that all enemies should have
	UCharacterClassInfo* CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	for (const TSubclassOf<UGameplayAbility> ClassAbility : CharacterClassInfo->CommonAbilities)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(ClassAbility, 1));
	}
}
