// vib


#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const APawn* OwningPawn = AIOwner->GetPawn();
	const FName TargetTag = FName("Player");
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, ActorsWithTag);

	float BestDistance = TNumericLimits<float>::Max();
	AActor* BestActor = nullptr;

	for (AActor* Actor : ActorsWithTag)
	{
		if (!IsValid(Actor)) continue;
		if (!Actor->Implements<UCombatInterface>()) continue;
		if (ICombatInterface::Execute_IsDead(Actor)) continue;
		const float Distance = OwningPawn->GetDistanceTo(Actor);
		if (Distance < BestDistance)
		{
			BestDistance = Distance;
			BestActor = Actor;
		}
	}
	// if (BestDistance > 1000.f) BestActor = nullptr;
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, BestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, BestDistance);
}
