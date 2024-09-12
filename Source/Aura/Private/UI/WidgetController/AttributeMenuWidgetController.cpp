// vib


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
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

	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnAttributePointsChanged.AddLambda(
		[this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		});
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	
	for (const FAuraAttributeInfo& Info : AttributeInfo->AttributeInformation)
	{
		FAuraAttributeInfo UpdatedInfo = Info;
		UpdatedInfo.AttributeValue = UpdatedInfo.Attribute.GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(UpdatedInfo);
	}
}
