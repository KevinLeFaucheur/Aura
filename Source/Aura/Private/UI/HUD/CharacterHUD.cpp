


#include "UI/HUD/CharacterHUD.h"
#include "UI/Widget/CharacterUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* ACharacterHUD::GetOverlayWidgetController(
	const FWidgetControllerParams& WidgetControllerParams)
{
	if(OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WidgetControllerParams);
		OverlayWidgetController->BindCallbacksToDependencies();
		
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void ACharacterHUD::InitOverlay(APlayerController* InPlayerController, APlayerState* InPlayerstate,
	UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized in BP_CharacterHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized in BP_CharacterHUD"));
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UCharacterUserWidget>(Widget);
	
	const FWidgetControllerParams WidgetControllerParams(InPlayerController, InPlayerstate, InAbilitySystemComponent, InAttributeSet);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	
	Widget->AddToViewport();
}
