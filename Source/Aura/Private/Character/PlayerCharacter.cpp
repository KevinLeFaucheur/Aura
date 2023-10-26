


#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/CharacterPlayerState.h"

APlayerCharacter::APlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void APlayerCharacter::InitAbilityActorInfo()
{
	ACharacterPlayerState* CharacterPlayerState = GetPlayerState<ACharacterPlayerState>();
	check(CharacterPlayerState);
	CharacterPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CharacterPlayerState, this);
	AbilitySystemComponent = CharacterPlayerState->GetAbilitySystemComponent();
	AttributeSet = CharacterPlayerState->GetAttributeSet();
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// InitAbilityActorInfo for Server
	InitAbilityActorInfo();
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// InitAbilityActorInfo for Client
	InitAbilityActorInfo();
}
