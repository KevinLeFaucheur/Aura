


#include "Character/PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/ARPGAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/CharacterPlayerController.h"
#include "Player/CharacterPlayerState.h"
#include "UI/HUD/CharacterHUD.h"

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
	Cast<UARPGAbilitySystemComponent>(CharacterPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = CharacterPlayerState->GetAbilitySystemComponent();
	AttributeSet = CharacterPlayerState->GetAttributeSet();

	if (ACharacterPlayerController* CharacterPlayerController = Cast<ACharacterPlayerController>(GetController()))
	{
		if (ACharacterHUD* CharacterHUD = Cast<ACharacterHUD>(CharacterPlayerController->GetHUD()))
		{
			CharacterHUD->InitOverlay(CharacterPlayerController, CharacterPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializePrimaryAttributes();
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
