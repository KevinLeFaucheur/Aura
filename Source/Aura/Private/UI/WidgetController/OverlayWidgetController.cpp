


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/ARPGAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UARPGAttributeSet* ARPGAttributeSet = CastChecked<UARPGAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(ARPGAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(ARPGAttributeSet->GetMaxHealth());
}

void UOverlayWidgetController::BindCallbacksToDependdencies()
{
	const UARPGAttributeSet* ARPGAttributeSet = CastChecked<UARPGAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ARPGAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ARPGAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
