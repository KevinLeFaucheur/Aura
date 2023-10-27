


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/ARPGAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UARPGAttributeSet* ARPGAttributeSet = CastChecked<UARPGAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(ARPGAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(ARPGAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(ARPGAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(ARPGAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependdencies()
{
	const UARPGAttributeSet* ARPGAttributeSet = CastChecked<UARPGAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ARPGAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ARPGAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ARPGAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ARPGAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
