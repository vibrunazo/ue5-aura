// vib


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogError) const
{
	for (FAuraInputAction Action : AbilityInputActions)
	{
		if (Action.InputAction&& Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	if (bLogError)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find input action for tag %s"), *InputTag.ToString());
	}
	return nullptr;
}
