


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/ARPGAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/CharacterPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetCharacterPlayerState()->GetSpellPoints());
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

	GetCharacterPlayerState()->OnSpellPointsChangedDelegate.AddLambda(
		[this] (int32 SpellsPoints)
		{
			SpellPointsChanged.Broadcast(SpellsPoints);
		});
}
