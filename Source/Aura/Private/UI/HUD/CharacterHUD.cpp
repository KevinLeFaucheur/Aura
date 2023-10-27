


#include "UI/HUD/CharacterHUD.h"
#include "UI/Widget/CharacterUserWidget.h"

void ACharacterHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}
