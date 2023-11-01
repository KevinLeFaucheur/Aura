


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/ARPGAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "ARPGGameplayTags.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UARPGAttributeSet* AS = CastChecked<UARPGAttributeSet>(AttributeSet);

	check(AttributeInfo);

	/*FARPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FARPGGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);*/
	
	for (auto& Pair : AS->TagsToAttributes)
	{
		FARPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		//Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		Info.AttributeValue = Pair.Value.GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}
