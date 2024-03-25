// vib


#include "Interaction/CombatInterface.h"

// Add default functionality here for any ICombatInterface functions that are not pure virtual.
int32 ICombatInterface::GetPlayerLevel()
{
	return 1;
}

FVector ICombatInterface::GetCombatSocketLocation()
{
	return FVector();
}
