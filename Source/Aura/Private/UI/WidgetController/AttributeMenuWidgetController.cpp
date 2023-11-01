


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/ARPGAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UARPGAttributeSet* AS = CastChecked<UARPGAttributeSet>(AttributeSet);
	check(AttributeInfo);	
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
        	[this, Pair](const FOnAttributeChangeData& Data)
        	{
        		BroadcastAttributeInfo(Pair.Key, Pair.Value);
        	}
        );
	}

}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UARPGAttributeSet* AS = CastChecked<UARPGAttributeSet>(AttributeSet);
	check(AttributeInfo);	
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FARPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
