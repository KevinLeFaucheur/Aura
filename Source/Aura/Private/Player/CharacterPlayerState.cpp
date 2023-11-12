


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
}

void ACharacterPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void ACharacterPlayerState::OnRep_XP(int32 InXP)
{
	OnXPChangedDelegate.Broadcast(XP);
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

void ACharacterPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

UAbilitySystemComponent* ACharacterPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
