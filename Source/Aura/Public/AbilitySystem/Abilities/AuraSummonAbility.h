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

protected:
	/** If dDebug is true. Draws a debug sphere. */
	void TryDrawDebugSphere(const FVector& Location, FColor Color, float Radius = 5.0f) const;
	
	/** If dDebug is true. Draws a debug sphere. */
	void TryDrawDebugArrow(const FVector& From, const FVector& To, FColor Color) const;
	
};
