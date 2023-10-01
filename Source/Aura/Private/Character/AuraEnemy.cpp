// vib


#include "Character/AuraEnemy.h"

void AAuraEnemy::HighlightActor()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Highlighted %s"), *GetName()));
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector(50, 50, 50), FColor::Yellow, false, 2);
	
}

void AAuraEnemy::UnHighlightActor()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, FString::Printf(TEXT("UnHighlighted %s"), *GetName()));
}
