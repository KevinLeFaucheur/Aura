

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "ProjectileSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UProjectileSpell : public UBaseGameplayAbility
{
	GENERATED_BODY()


protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
};
