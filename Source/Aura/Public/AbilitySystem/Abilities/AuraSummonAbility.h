// vib

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Summoning")
	TArray<FVector> GetSpawnLocations();

	/**
	 * Calls SpawnSingle after given delay.
	 */
	UFUNCTION(BlueprintCallable, Category = "Summoning")
	void DelayedSpawnSingle(const float Delay = 0.0f, const FVector Location = FVector::ZeroVector);

	/**
	 * Implement logic for spawning a single minion in BPs here. Called by DelayedSpawn.
	 */
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Summoning")
	void SpawnSingle(const FVector Location);

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 500.0f;

	/** The angle for the spawning cone in degrees. */
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpread = 90.0f;

	/** If true, draws debug lines and spheres showing the spawn area and chosen spawn locations. */
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	bool bDebug = false;

	/**
	 * Returns a random class from MinionClasses
	 * @return a random class from MinionClasses
	 */
	UFUNCTION(BlueprintPure, Category = "Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();

protected:
	/** If dDebug is true. Draws a debug sphere. */
	void TryDrawDebugSphere(const FVector& Location, FColor Color, float Radius = 5.0f) const;
	
	/** If dDebug is true. Draws a debug sphere. */
	void TryDrawDebugArrow(const FVector& From, const FVector& To, FColor Color) const;
	
};
