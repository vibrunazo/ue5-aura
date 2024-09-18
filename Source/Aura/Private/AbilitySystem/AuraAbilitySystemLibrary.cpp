// vib


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "Character/AuraCharacterBase.h"
#include "Engine/OverlapResult.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetControllerParams, AAuraHUD*& OutAuraHUD)
{
	APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController();
	if (!PC) return false;
	OutAuraHUD = Cast<AAuraHUD>(PC->GetHUD());
	if (!OutAuraHUD) return false;
	AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
	UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
	UAttributeSet *AS = PS->GetAttributeSet();
	OutWidgetControllerParams.AttributeSet = AS;
	OutWidgetControllerParams.PlayerController = PC;
    OutWidgetControllerParams.PlayerState = PS;
    OutWidgetControllerParams.AbilitySystemComponent = ASC;
    return true;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;
	APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController();
	if (!PC) return nullptr;
	AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD());
	if (!AuraHUD) return nullptr;
	return AuraHUD->GetOverlayWidgetController();
	// AAuraHUD* AuraHUD = nullptr;
	// FWidgetControllerParams WCParams;
	// if (!MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD)) return nullptr;
	// return AuraHUD->GetOverlayWidgetController(WCParams);
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;
	APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController();
	if (!PC) return nullptr;
	AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD());
	if (!AuraHUD) return nullptr;
	return AuraHUD->GetAttributeMenuWidgetController();
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	if (!WorldContextObject) return nullptr;
	APlayerController* PC = WorldContextObject->GetWorld()->GetFirstPlayerController();
	if (!PC) return nullptr;
	AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD());
	if (!AuraHUD) return nullptr;
	return AuraHUD->GetSpellMenuWidgetController();
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
	// Gets the Asset that contains the GameplayEffect class that sets the initial Primary Attributes for all Character Classes
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	// Gets the struct that contains that GE for this given Class
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	const TSubclassOf<UGameplayEffect> PrimaryAttributesEffectClass = ClassDefaultInfo.PrimaryAttributes;
	const TSubclassOf<UGameplayEffect> SecondaryAttributesEffectClass = CharacterClassInfo->SecondaryAttributes;
	const TSubclassOf<UGameplayEffect> VitalAttributesEffectClass = CharacterClassInfo->VitalAttributes;

	ApplyEffectFromClass(PrimaryAttributesEffectClass, ASC, Level);
	ApplyEffectFromClass(SecondaryAttributesEffectClass, ASC, Level);
	ApplyEffectFromClass(VitalAttributesEffectClass, ASC, Level);
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	// Gets the Asset that contains the initial Abilities that all enemies should have
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo) return;
	for (const TSubclassOf<UGameplayAbility> ClassAbility : CharacterClassInfo->CommonAbilities)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(ClassAbility, 1));
	}
	FCharacterClassDefaultInfo ClassStruct = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	for (const TSubclassOf<UGameplayAbility> ClassAbility : ClassStruct.ClassAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			ASC->GiveAbility(FGameplayAbilitySpec(ClassAbility, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor())));
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	return AuraGameMode->CharacterClassInfo;
}

int32 UAuraAbilitySystemLibrary::GetXPRewarded(const UObject* WorldContextObject, ECharacterClass CharacterClass,
	float Level)
{
	// Gets the Asset that contains the GameplayEffect class that sets the initial Primary Attributes for all Character Classes
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo) return 0;
	// Gets the Class Info specific for the given Class
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	const float XPRewarded = ClassDefaultInfo.XPRewarded.GetValueAtLevel(Level);

	return static_cast<int32>(XPRewarded);
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameplayEffectContext* Context = EffectContextHandle.Get();
	const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(Context);
	if (!AuraContext) return false;
	return AuraContext->IsBlockedHit();
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGameplayEffectContext* Context = EffectContextHandle.Get();
	const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(Context);
	if (!AuraContext) return false;
	return AuraContext->IsCriticalHit();
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	FGameplayEffectContext* Context = EffectContextHandle.Get();
	FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(Context);
	if (!AuraContext) return;
	AuraContext->SetIsBlockedHit(bInIsBlockedHit);
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	FGameplayEffectContext* Context = EffectContextHandle.Get();
	FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(Context);
	if (!AuraContext) return;
	AuraContext->SetIsCriticalHit(bInIsCriticalHit);
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutLivePlayers, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& Origin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	SphereParams.MobilityType = EQueryMobilityType::Dynamic;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);

		for (const FOverlapResult& Overlap : Overlaps)
		{
			AActor* OverlapActor = Overlap.GetActor();
			if (!IsValid(OverlapActor)) continue;
			if (OverlapActor->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(OverlapActor))
			{
				OutLivePlayers.AddUnique(OverlapActor);
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsEnemyTeam(AActor* FirstActor, AActor* SecondActor)
{
	if (!IsValid(FirstActor) || !IsValid(SecondActor)) return false;
	if (FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Enemy"))) return true;
	if (FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Player"))) return true;
	return false;
}
