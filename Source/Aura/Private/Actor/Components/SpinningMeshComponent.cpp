// vib


#include "Actor/Components/SpinningMeshComponent.h"

USpinningMeshComponent::USpinningMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USpinningMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	AddLocalRotation(RotationSpeed * DeltaTime);
}
