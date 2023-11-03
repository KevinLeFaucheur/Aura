


#include "AbilitySystem/Abilities/ProjectileSpell.h"

#include "Actor/Projectile.h"
#include "Interaction/CombatInterface.h"


void UProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

	if(!bIsServer) return;;

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if(CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;
		;
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		
		AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
			 ProjectileClass,
			 SpawnTransform,
			 GetOwningActorFromActorInfo(),
			 Cast<APawn>(GetOwningActorFromActorInfo()),
			 ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// TODO: Give the projectile a Gameplay Effect Spec for causing damage
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
