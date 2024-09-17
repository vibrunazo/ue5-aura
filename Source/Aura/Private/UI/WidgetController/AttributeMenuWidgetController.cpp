// vib


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);

	for (const FAuraAttributeInfo& Info : AttributeInfo->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Info.Attribute).AddLambda(
			[this, Info](const FOnAttributeChangeData& Data)
			{
				FAuraAttributeInfo UpdatedInfo = Info;
				UpdatedInfo.AttributeValue = Data.NewValue;
				AttributeInfoDelegate.Broadcast(UpdatedInfo);
			}
		);
	}

	GetAuraPS()->OnAttributePointsChanged.AddLambda(
		[this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		});
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);
	
	for (const FAuraAttributeInfo& Info : AttributeInfo->AttributeInformation)
	{
		FAuraAttributeInfo UpdatedInfo = Info;
		UpdatedInfo.AttributeValue = UpdatedInfo.Attribute.GetNumericValue(GetAuraAS());
		AttributeInfoDelegate.Broadcast(UpdatedInfo);
	}

	AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(AttributeTag);
}
