// vib


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	if (NumMinions == 0.f) return TArray<FVector>();
	const FVector ForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	float DeltaSpread = SpawnSpread;
	if (NumMinions > 1.f) DeltaSpread = DeltaSpread / (NumMinions - 1.f);
	

	const FVector LeftOfSpread = ForwardVector.RotateAngleAxis(-SpawnSpread / 2.0f, FVector::UpVector);
	TryDrawDebugArrow(Location + FVector(0, 0, 10), Location + LeftOfSpread * MaxSpawnDistance, FColor::Green);
	TryDrawDebugSphere(Location + LeftOfSpread * MinSpawnDistance, FColor::Red);
	TryDrawDebugSphere(Location + LeftOfSpread * MaxSpawnDistance, FColor::Red);
	
	const FVector RightOfSpread = ForwardVector.RotateAngleAxis(SpawnSpread / 2.0f, FVector::UpVector);
	TryDrawDebugArrow(Location + FVector(0, 0, 10), Location + RightOfSpread * MaxSpawnDistance, FColor::Blue);
	TryDrawDebugSphere(Location + RightOfSpread * MinSpawnDistance, FColor::Red);
	TryDrawDebugSphere(Location + RightOfSpread * MaxSpawnDistance, FColor::Red);
	TArray<FVector> SpawnLocations;
	
	// loop through the number of minions
	for (int32 i = 0; i < NumMinions; i++)
	{
		// draw a debug arrow for each line by adding the delta spread angle to the left of the spread
		const FVector Direction = LeftOfSpread.RotateAngleAxis(i * DeltaSpread, FVector::UpVector);
		TryDrawDebugArrow(Location, Location + Direction * MaxSpawnDistance, FColor::White);
		const FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		TryDrawDebugSphere(ChosenSpawnLocation, FColor::Magenta);
		SpawnLocations.Add(ChosenSpawnLocation);
		
	}
	
	return SpawnLocations;
}

void UAuraSummonAbility::TryDrawDebugSphere(const FVector& Location, const FColor Color, const float Radius) const
{
	if (!bDebug) return;
	UKismetSystemLibrary::DrawDebugSphere(GetAvatarActorFromActorInfo(), Location, Radius, 12, Color, 5.0f);
}

void UAuraSummonAbility::TryDrawDebugArrow(const FVector& From, const FVector& To, FColor Color) const
{
	if (!bDebug) return;
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), From, To, 15.0f, Color, 5.0f, 5.0f);
}
