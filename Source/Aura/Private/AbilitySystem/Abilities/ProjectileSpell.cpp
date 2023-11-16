


#include "AbilitySystem/Abilities/ProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/Projectile.h"
#include "Interaction/CombatInterface.h"
#include "ARPGGameplayTags.h"


void UProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

}

void UProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();

	if(!bIsServer) return;;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if(bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
		 ProjectileClass,
		 SpawnTransform,
		 GetOwningActorFromActorInfo(),
		 Cast<APawn>(GetOwningActorFromActorInfo()),
		 ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);
		
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

	const FARPGGameplayTags GameplayTags = FARPGGameplayTags::Get();

	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}
	Projectile->DamageEffectSpecHandle = SpecHandle;
	Projectile->FinishSpawning(SpawnTransform);
}

FString UProjectileSpell::GetDescription(int32 Level)
{
	const FString NumOfProjectiles =  Level == 1 ? FString("a bolt") : FString::Printf(TEXT("%d bolts"), FMath::Min(NumProjectiles, Level));
	const int32 Damage = DamageTypes[FARPGGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n<Default>Launches %s of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn.</>\n\n<Small>Level: </><Level>%d</>"), *NumOfProjectiles, Damage, Level);
}

FString UProjectileSpell::GetNextLevelDescription(int32 Level)
{
	const FString NumOfProjectiles =  Level == 1 ? FString("a bolt") : FString::Printf(TEXT("%d bolts"), FMath::Min(NumProjectiles, Level));
	const int32 Damage = DamageTypes[FARPGGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);
	return FString::Printf(TEXT("<Title>NEXT LEVEL</>\n\n<Default>Launches %s of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn.</>\n\n<Small>Level: </><Level>%d</>"), *NumOfProjectiles, Damage, Level);
}