// vib


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::CastProjectile(const FVector& TargetLocation)
{
	if (!GetAvatarActorFromActorInfo() || !GetWorld()) return;
	if (!ProjectileClass) return;
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		AuraTags::CombatSocket_Weapon);
	
	FVector UpdatedTarget = TargetLocation;
	// FVector UpdatedTarget = FVector(TargetLocation.X, TargetLocation.Y, GetAvatarActorFromActorInfo()->GetActorLocation().Z);
	const float Distance = (UpdatedTarget - GetAvatarActorFromActorInfo()->GetActorLocation()).Length();
	// If the target is too close to the caster, then target a little bit further behind the target to avoid casting backwards.
	// Casting backwards would happen if the target is between the SocketLocation and the caster Location
	if (Distance <= 200.f)
	{
		UpdatedTarget += (UpdatedTarget - GetAvatarActorFromActorInfo()->GetActorLocation()).GetSafeNormal() * 120.f;
	}
	FRotator Rotation = (UpdatedTarget - SocketLocation).Rotation();

	if (Rotation.Pitch < 0)	Rotation.Pitch = 0.f;
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
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
		GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(Projectile);
	FHitResult HitResult;
	HitResult.Location = TargetLocation;
	ContextHandle.AddHitResult(HitResult);
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(
		DamageEffectClass, GetAbilityLevel(), ContextHandle);

	for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageTypes)
	{
		const FGameplayTag& Tag = Pair.Key;
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tag, ScaledDamage);
	}

	Projectile->DamageEffectSpecHandle = SpecHandle;
	
	Projectile->FinishSpawning(SpawnTransform);
}

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
