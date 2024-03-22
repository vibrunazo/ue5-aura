// vib


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogError) const
{
	for (const FAuraAttributeInfo& AttributeInfo : AttributeInformation)
	{
		if (AttributeInfo.AttributeTag == AttributeTag)
		{
			return AttributeInfo;
		}
	}
	if (bLogError)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not find attribute info for tag: %s on AttributeInfo: %s"),
			*AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FAuraAttributeInfo(); 
}
