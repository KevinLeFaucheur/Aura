

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AEnemyCharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	//~ IEnemyInterface 
	virtual void HighlightActor()  override;
	virtual void UnhighlightActor() override;
	//~ IEnemyInterface

	//~ CombatInterface
	virtual int32 GetPlayerLevel() override;
	//~ CombatInterface

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;
};
