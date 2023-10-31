


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
}

void ACharacterPlayerState::OnRep_Level(int32 OldLevel)
{
	
}

UAbilitySystemComponent* ACharacterPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
