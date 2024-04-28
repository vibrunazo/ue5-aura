// vib

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;
class UAnimMontage;
/**
 * Struct that maps a Montage to a MontageTag. Used by abilities to determine which Montage to play given
 * a Tag. Each Character has an Array of these Structs, so they can choose
 * which Montage to play for each Tag.
 */
USTRUCT(BlueprintType)
struct FTaggedMontage
{
	
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetPlayerLevel();
	/**
	 * Returns the location of the socket based on the tag. The Character will have an Array of
	 * Montages that it can choose from. Which are mapped to a MontageTag. The Ability will ask the Character for
	 * which Montage to play. Then use this function to know which socket to put the Damage sphere on.
	 * @param SocketTag Tag related to the socket
	 * @return The location of the socket
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	FVector GetCombatSocketLocation(const FGameplayTag& SocketTag);
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "CombatInterface")
	void SetMotionWarpTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	TArray<FTaggedMontage> GetAttackMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	UNiagaraSystem* GetBloodEffect();

	/**
	 * Returns the TaggedMontage that has given MontageTag and its other relevant data. The MontageTag is used as an index
	 * to the Character's array of TaggedMontages. The TaggedMontage contains meta data related to that Montage. You could
	 * use this function to get the ImpactSound that should be played for a given Montage.
	 * @param MontageTag MontageTag to look up a TaggedMontage with. 
	 * @return TaggedMontage that contains the given MontageTag plus the SocketTag, Montage and ImpactSound
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	/**
	 * Returns the number of Minions that the Character has.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "CombatInterface")
	int32 GetMinionCount();
};
