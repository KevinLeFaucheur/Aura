


#include "AbilitySystem/ARPGAttributeSet.h"
#include "Net/UnrealNetwork.h"

UARPGAttributeSet::UARPGAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(35.f);
	InitMaxMana(100.f);
}

void UARPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UARPGAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UARPGAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UARPGAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UARPGAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UARPGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UARPGAttributeSet, Health, OldHealth);
}

void UARPGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UARPGAttributeSet, MaxHealth, OldMaxHealth);
}

void UARPGAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UARPGAttributeSet, Mana, OldMana);
}

void UARPGAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UARPGAttributeSet, MaxMana, OldMaxMana);
}
