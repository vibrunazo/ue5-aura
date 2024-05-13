// vib

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

/**
 * Struct containing Info for leveling up from on specific level to the next.
 */
USTRUCT(BlueprintType)
struct FSingleLevelInfo
{
	GENERATED_BODY()

	/** XP required to level up. **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelUpRequirement = 0;
	
	/** Attribute points rewarded on level up. **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AttributePointReward = 1;
	
	/** Spell points rewarded on level up. **/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SpellPointReward = 1;
	
};
/**
 *  Data Asset containing info for leveling up for all levels. Has an Array of FSingleLevelInfo structs,
 *  which contains info for each level.
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Array with a FSingleLevelInfo struct for each level. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Up Information")
	TArray<FSingleLevelInfo> AllLevelsInfo;

	/**
	 * Finds the level corresponding to the given experience points (XP).
	 *
	 * This function searches through the level information data to determine
	 * which level is reached with the provided amount of XP. The levels are
	 * defined in the AllLevelsInfo array which contains FSingleLevelInfo structs
	 * for each level, indicating the XP required to reach that level.
	 *
	 * @param XP The amount of experience points to find the corresponding level for.
	 * @return The level number that the given XP reaches. If the XP does not reach
	 *         any level, returns 1.
	 * @note This function assumes that the AllLevelsInfo array is sorted in ascending order
	 *       by the XP required to reach each level.
	 */
	UFUNCTION(BlueprintCallable, Category = "Level Up Information")
	int32 FindLevelForXP(int32 XP) const;
	
};
