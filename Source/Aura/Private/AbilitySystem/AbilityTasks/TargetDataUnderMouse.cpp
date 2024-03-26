// vib


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "Player/AuraPlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const APawn* OwnerPawn = Cast<APawn>(Ability->GetAvatarActorFromActorInfo());
	// if (!OwnerPawn) return;
	const AAuraPlayerController* Controller = Cast<AAuraPlayerController>(OwnerPawn->Controller);
	// if (!Controller) return;
	// FHitResult CursorHit;
	FHitResult CursorHit = Controller->CursorHit;
	// Controller->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	// if (!CursorHit.bBlockingHit) return;
	ValidData.Broadcast(CursorHit.ImpactPoint);
	
}
