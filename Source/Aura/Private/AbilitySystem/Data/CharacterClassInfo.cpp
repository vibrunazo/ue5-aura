// vib


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInfoMap.FindChecked(CharacterClass); 
}
