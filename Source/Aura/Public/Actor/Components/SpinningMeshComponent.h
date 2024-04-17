// vib

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "SpinningMeshComponent.generated.h"

/**
 * A Static Mesh Component that spins constantly according to given RotationSpeed.
 */
UCLASS(Blueprintable, ClassGroup=(Rendering, Common), hidecategories=(Object,Activation,"Components|Activation"), ShowCategories=(Mobility), editinlinenew, meta=(BlueprintSpawnableComponent), MinimalAPI)
class USpinningMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	USpinningMeshComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	/** How fast the Mesh Rotates. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile")
	FRotator RotationSpeed = FRotator::ZeroRotator;
	
};
