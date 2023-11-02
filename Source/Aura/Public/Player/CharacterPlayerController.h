

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "CharacterPlayerController.generated.h"

class UARPGInputConfig;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UARPGAbilitySystemComponent;
class USplineComponent;
/**
 * 
 */
UCLASS()
class AURA_API ACharacterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACharacterPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext>	CharacterContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);
	void AutoRun();
	void CursorTrace();
	
	FHitResult CursorHit;
	TObjectPtr<IEnemyInterface> LastActor;
	TObjectPtr<IEnemyInterface> ThisActor;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UARPGInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UARPGAbilitySystemComponent> ARPGAbilitySystemComponent;

	UARPGAbilitySystemComponent* GetASC();

	/*
	 * Click To Move
	 */
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float ShortPressThreshold = 0.5f;

	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
};
