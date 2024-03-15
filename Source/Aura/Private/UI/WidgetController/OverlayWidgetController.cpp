// vib


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

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

	// Bind the effect asset tags to the ability system component
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
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
}
