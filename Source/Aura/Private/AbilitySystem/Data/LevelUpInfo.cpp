// vib


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;

	// Iterate through all levels until we find where XP fits
	for (const FSingleLevelInfo& LevelInfo : AllLevelsInfo)
	{
		// if LevelUpRequirement is 0, the current Level is the placeholder level zero.
		if (LevelInfo.LevelUpRequirement == 0) continue;
		// if XP is less than the required for the next level, the current Level is the highest with given XP.
		if (XP < LevelInfo.LevelUpRequirement) break;
		++Level;
	}

	// Return the found level. If XP was below the first level, Level is 1.
	// If XP was higher than the highest level in AllLevelsInfo, Level is the highest level.
	return Level;
}
