

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "ARPGAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UARPGAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
