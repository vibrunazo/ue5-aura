// vib


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (const FAuraAttributeInfo& Info : AttributeInfo->AttributeInformation)
	{
		FAuraAttributeInfo UpdatedInfo = Info;
		UpdatedInfo.AttributeValue = UpdatedInfo.Attribute.GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(UpdatedInfo);
	}

	// FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	// Info.AttributeValue = AS->GetStrength();
	// AttributeInfoDelegate.Broadcast(Info);
}
