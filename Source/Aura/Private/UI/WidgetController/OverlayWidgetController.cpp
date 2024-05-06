// vib


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"

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

	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!AuraASC) return;

	if (AuraASC->bStartupAbilitiesGiven) OnInitializeStartupAbilities(AuraASC);
	else
		AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
	
	// Bind delegate to listen for effect changes for UI messages (ie "you collected a health potion")
	// and broadcast them for message widgets
	AuraASC->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC)
{
	if (!AuraASC->bStartupAbilitiesGiven) return;

	// TODO Get information about all abilities given, look up their ability info and broadcast it to the UI widgets
	GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Yellow, FString::Printf(TEXT("Startup Abilities given!")));

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(UAuraAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = UAuraAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
		
	});
	AuraASC->ExecForEachAbility(BroadcastDelegate);
	
}
