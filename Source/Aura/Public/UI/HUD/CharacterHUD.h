

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CharacterHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UCharacterUserWidget;
/**
 * 
 */
UCLASS()
class AURA_API ACharacterHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UCharacterUserWidget> OverlayWidget;

	UOverlayWidgetController*  GetOverlayWidgetController(const FWidgetControllerParams& WidgetControllerParams);

	void InitOverlay(APlayerController* InPlayerController, APlayerState* InPlayerstate, UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet);

protected:
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCharacterUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
};
