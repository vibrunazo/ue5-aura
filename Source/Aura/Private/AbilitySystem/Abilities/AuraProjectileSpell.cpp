// vib


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::CastProjectile(const FVector& TargetLocation)
{
	if (!GetAvatarActorFromActorInfo() || !GetWorld()) return;
	if (!ProjectileClass) return;
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FVector UpdatedTarget = FVector(TargetLocation.X, TargetLocation.Y, GetAvatarActorFromActorInfo()->GetActorLocation().Z);
		const float Distance = (UpdatedTarget - GetAvatarActorFromActorInfo()->GetActorLocation()).Length();
		// If the target is too close to the caster, then target a little bit further behind the target to avoid casting backwards.
		// Casting backwards would happen if the target is between the SocketLocation and the caster Location
		if (Distance <= 200.f)
		{
			UpdatedTarget += (UpdatedTarget - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal() * 120.f;
		}
		FRotator Rotation = (UpdatedTarget - SocketLocation).Rotation();
		
		Rotation.Pitch = 0.f;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		// Set the projectile's rotation
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		// TODO: Give the Projectile a GameplayEffect Spec to apply
		Projectile->FinishSpawning(SpawnTransform);
	}
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// CastProjectile();
}
