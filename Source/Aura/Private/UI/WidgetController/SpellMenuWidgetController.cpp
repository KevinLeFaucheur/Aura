


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/ARPGAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetARPGAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda(
		[this] (const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
		{
			if(AbilityInfo)
			{
				FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});
}
