


#include "AbilitySystem/ARPGAbilitySystemGlobals.h"

#include "AbilitySystemTypes.h"

FGameplayEffectContext* UARPGAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FARPGGameplayEffectContext();
}
