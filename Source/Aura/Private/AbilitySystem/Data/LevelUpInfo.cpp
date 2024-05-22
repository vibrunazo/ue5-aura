// vib


#include "AbilitySystem/Data/LevelUpInfo.h"


float ULevelUpInfo::GetPercentXPInCurrentLevel(int32 TotalXP) const
{
	float Percent = 0.f;
	int32 Level = 1;
	int32 XPInLevel = TotalXP;

	// Iterate through all levels until we find where XP fits
	for (const FSingleLevelInfo& LevelInfo : AllLevelsInfo)
	{
		if (LevelInfo.LevelUpRequirement == 0) continue;
		if (TotalXP < LevelInfo.LevelUpRequirement) break;
		++Level;
		XPInLevel = TotalXP - LevelInfo.LevelUpRequirement;
	}
	
	FSingleLevelInfo CurrentLevelInfo = AllLevelsInfo[Level];
	if (CurrentLevelInfo.LevelUpRequirement > 0)
	{
		Percent = static_cast<float>(XPInLevel) / static_cast<float>(CurrentLevelInfo.LevelUpRequirement);
	}
	
	return Percent;
}

float ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	float Level = 1.0f;

	// Iterate through all levels until we find where XP fits
	for (const FSingleLevelInfo& LevelInfo : AllLevelsInfo)
	{
		// if LevelUpRequirement is 0, the current Level is the placeholder level zero.
		if (LevelInfo.LevelUpRequirement == 0) continue;
		// if XP is less than the required for the next level, the current Level is the highest with given XP.
		if (XP < LevelInfo.LevelUpRequirement)
		{
			int32 LastLevelXP = AllLevelsInfo[Level - 1].LevelUpRequirement;
			Level += static_cast<float>(XP - LastLevelXP) / static_cast<float>(LevelInfo.LevelUpRequirement - LastLevelXP);
			break;
		}
		++Level;
	}

	// Return the found level. If XP was below the first level, Level is 1.
	// If XP was higher than the highest level in AllLevelsInfo, Level is the highest level.
	return Level;
}
