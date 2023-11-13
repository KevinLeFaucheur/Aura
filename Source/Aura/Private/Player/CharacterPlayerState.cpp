


#include "Player/CharacterPlayerState.h"

#include "AbilitySystem/ARPGAbilitySystemComponent.h"
#include "AbilitySystem/ARPGAttributeSet.h"
#include "Net/UnrealNetwork.h"

ACharacterPlayerState::ACharacterPlayerState()
{

	AbilitySystemComponent = CreateDefaultSubobject<UARPGAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UARPGAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;
}

void ACharacterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACharacterPlayerState, Level);
	DOREPLIFETIME(ACharacterPlayerState, XP);
	DOREPLIFETIME(ACharacterPlayerState, AttributePoints);
	DOREPLIFETIME(ACharacterPlayerState, SpellPoints);
}

void ACharacterPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void ACharacterPlayerState::OnRep_XP(int32 InXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void ACharacterPlayerState::OnRep_AttributePoints(int32 InAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ACharacterPlayerState::OnRep_SpellPoints(int32 InSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ACharacterPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ACharacterPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ACharacterPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ACharacterPlayerState::AddToAttributePoints(int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ACharacterPlayerState::AddToSpellPoints(int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void ACharacterPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

UAbilitySystemComponent* ACharacterPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
