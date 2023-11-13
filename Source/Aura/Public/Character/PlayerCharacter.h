

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interaction/PlayerInterface.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class AURA_API APlayerCharacter : public ABaseCharacter, public IPlayerInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//~ CombatInterface
	virtual int32 GetPlayerLevel_Implementation() override;
	//~ CombatInterface

	//~ IPlayerInterface
	virtual  int32 GetXP_Implementation() const override;
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InLevel) const override;
	virtual void AddToAttributePointsReward_Implementation(int32 InSpellPoints) const override;
	virtual void AddToSpellPointsReward_Implementation(int32 InSpellPoints) const override;
	//~ IPlayerInterface

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
};
