// vib


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributes = CastChecked<UAuraAttributeSet>(AttributeSet);
	if (!AuraAttributes) return;
	OnMaxHealthChanged.Broadcast(AuraAttributes->GetMaxHealth());
	OnHealthChanged.Broadcast(AuraAttributes->GetHealth());
	OnMaxManaChanged.Broadcast(AuraAttributes->GetMaxMana());
	OnManaChanged.Broadcast(AuraAttributes->GetMana());
}

// Helper function to bind a single attribute change delegate to a broadcast function
void UOverlayWidgetController::BindAttributeChangeDelegate(const FGameplayAttribute& Attribute, 
	FOnAttributeChangedSignature* BroadcastDelegate) const
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute).AddLambda(
		[this, BroadcastDelegate](const FOnAttributeChangeData& Data)
		{
			BroadcastDelegate->Broadcast(Data.NewValue);
		});
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributes = CastChecked<UAuraAttributeSet>(AttributeSet);

	// Bind the attribute change delegates
	BindAttributeChangeDelegate(AuraAttributes->GetHealthAttribute(), &OnHealthChanged);
	BindAttributeChangeDelegate(AuraAttributes->GetMaxHealthAttribute(), &OnMaxHealthChanged);
	BindAttributeChangeDelegate(AuraAttributes->GetManaAttribute(), &OnManaChanged);
	BindAttributeChangeDelegate(AuraAttributes->GetMaxManaAttribute(), &OnMaxManaChanged);

	if (!GetAuraASC()) return;

	if (GetAuraASC()->bStartupAbilitiesGiven) BroadcastAbilityInfo();
	else
		GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
	
	// Bind delegate to listen for effect changes for UI messages (ie "you collected a health potion")
	// and broadcast them for message widgets
	GetAuraASC()->EffectAssetTagsChangedDelegate.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
	if (GetAuraPS())
	{
		GetAuraPS()->OnPlayerLevelChanged.AddLambda(
			[this](int32 NewLevel)
			{
				OnPlayerLevelChanged.Broadcast(NewLevel);
			}
		);

		GetAuraPS()->OnXPChanged.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	float NewLevel = GetAuraPS()->LevelUpInfo->FindLevelForXP(NewXP);
	// gets the decimal part of the Level, example, Level = 9.3, Percent = 0.3
	float Percent = NewLevel - FMath::Floor(NewLevel);
	OnXPPercentChangedDelegate.Broadcast(Percent);
}
