// vib

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 *
 * Singleton containing native GameplayTags.
 */
struct FAuraGameplayTags
{
public:
	static FAuraGameplayTags& Get()	{ return GameplayTags; }
	static void InitializeNativeGameplayTags();

protected:

private:
	static FAuraGameplayTags GameplayTags; 
};