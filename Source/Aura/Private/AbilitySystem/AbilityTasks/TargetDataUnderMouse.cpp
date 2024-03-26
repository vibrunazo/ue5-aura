// vib


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "Player/AuraPlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	if (const bool bIsLocallyControlled = Ability->IsLocallyControlled())
	{
		SendMouseCursorData();
	}
	else
	{
		// TODO: We are on the server, so listen to target data
	}
	
	
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	
	const APawn* OwnerPawn = Cast<APawn>(Ability->GetAvatarActorFromActorInfo());
	if (!OwnerPawn) return;
	const AAuraPlayerController* Controller = Cast<AAuraPlayerController>(OwnerPawn->Controller);
	if (!Controller) return;
	const FHitResult CursorHit = Controller->CursorHit;
	if (!CursorHit.bBlockingHit) return;
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);
	
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
