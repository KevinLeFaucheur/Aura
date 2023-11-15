


#include "Character/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/ARPGAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CharacterPlayerController.h"
#include "Player/CharacterPlayerState.h"
#include "UI/HUD/CharacterHUD.h"

APlayerCharacter::APlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Level Up Niagara Component"));
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CharacterClass = ECharacterClass::Elementalist;

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
	InitializeDefaultAttributes();
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// InitAbilityActorInfo for Server
	InitAbilityActorInfo();
	AddCharacterAbilities(); // Both Active and Passive
}

void APlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// InitAbilityActorInfo for Client
	InitAbilityActorInfo();
}

int32 APlayerCharacter::GetPlayerLevel_Implementation()
{
	const ACharacterPlayerState* CharacterPlayerState = GetPlayerState<ACharacterPlayerState>();
	check(CharacterPlayerState);
	return CharacterPlayerState->GetPlayerLevel();
}

int32 APlayerCharacter::GetXP_Implementation() const
{
	const ACharacterPlayerState* CharacterPlayerState = GetPlayerState<ACharacterPlayerState>();
	check(CharacterPlayerState);
	return CharacterPlayerState->GetXP();
}

void APlayerCharacter::AddToXP_Implementation(int32 InXP)
{
	ACharacterPlayerState* CharacterPlayerState = GetPlayerState<ACharacterPlayerState>();
	check(CharacterPlayerState);
	CharacterPlayerState->AddToXP(InXP);
}

int32 APlayerCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const ACharacterPlayerState* CharacterPlayerState = GetPlayerState<ACharacterPlayerState>();
	check(CharacterPlayerState);
	return CharacterPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 APlayerCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const ACharacterPlayerState* CharacterPlayerState = GetPlayerState<ACharacterPlayerState>();
	check(CharacterPlayerState);
	return CharacterPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointReward;
}

int32 APlayerCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const ACharacterPlayerState* CharacterPlayerState = GetPlayerState<ACharacterPlayerState>();
	check(CharacterPlayerState);
	return CharacterPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointReward;
}

void APlayerCharacter::AddToPlayerLevel_Implementation(int32 InLevel) const
{
	ACharacterPlayerState* CharacterPlayerState = GetPlayerState<ACharacterPlayerState>();
	check(CharacterPlayerState);
	CharacterPlayerState->AddToLevel(InLevel);

	if (UARPGAbilitySystemComponent* ARPGASC = Cast<UARPGAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ARPGASC->UpdateAbilityStatuses(CharacterPlayerState->GetPlayerLevel());
	}
}

void APlayerCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints) const
{
	ACharacterPlayerState* CharacterPlayerState = GetPlayerState<ACharacterPlayerState>();
	check(CharacterPlayerState);
	CharacterPlayerState->AddToAttributePoints(InAttributePoints);
}

void APlayerCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints) const
{
	ACharacterPlayerState* CharacterPlayerState = GetPlayerState<ACharacterPlayerState>();
	check(CharacterPlayerState);
	CharacterPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 APlayerCharacter::GetAttributePoints_Implementation() const
{
	ACharacterPlayerState* CharacterPlayerState = GetPlayerState<ACharacterPlayerState>();
	check(CharacterPlayerState);
	return CharacterPlayerState->GetAttributePoints();
}

int32 APlayerCharacter::GetSpellPoints_Implementation() const
{
	ACharacterPlayerState* CharacterPlayerState = GetPlayerState<ACharacterPlayerState>();
	check(CharacterPlayerState);
	return CharacterPlayerState->GetSpellPoints();
}

void APlayerCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void APlayerCharacter::MulticastLevelUpParticles_Implementation() const
{
	if(IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = CameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}
