

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class AURA_API ACharacterEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACharacterEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	
	UPROPERTY(EditAnywhere, Category="Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
};
