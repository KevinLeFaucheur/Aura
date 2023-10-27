


#include "UI/Widget/CharacterUserWidget.h"

void UCharacterUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
