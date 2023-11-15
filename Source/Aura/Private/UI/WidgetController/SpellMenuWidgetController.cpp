


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "ARPGGameplayTags.h"
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
		[this] (const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{
			if(SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				bool bEnableSpendPointButton = false;
				bool bEnableEquipButton = false;
				ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPointButton, bEnableEquipButton);
				OnSpellGlobeSelectedDelegate.Broadcast(bEnableSpendPointButton, bEnableEquipButton);
			}
			
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
			CurrentSpellPoints = SpellsPoints;
			
			bool bEnableSpendPointButton = false;
			bool bEnableEquipButton = false;
			ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPointButton, bEnableEquipButton);
			OnSpellGlobeSelectedDelegate.Broadcast(bEnableSpendPointButton, bEnableEquipButton);
		});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FARPGGameplayTags GameplayTags = FARPGGameplayTags::Get();
	const int32 SpellPoints = GetCharacterPlayerState()->GetSpellPoints();
	FGameplayTag AbilityStatus;
	const FGameplayAbilitySpec* AbilitySpec = ARPGAbilitySystemComponent->GetSpecFromAbilityTag(AbilityTag);

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const bool bSpecValid = AbilitySpec != nullptr;
	if(!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetARPGAbilitySystemComponent()->GetStatusTagFromSpec(*AbilitySpec);	
	}
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	bool bEnableSpendPointButton = false;
	bool bEnableEquipButton = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPointButton, bEnableEquipButton);
	OnSpellGlobeSelectedDelegate.Broadcast(bEnableSpendPointButton, bEnableEquipButton);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if(GetARPGAbilitySystemComponent())
	{
		GetARPGAbilitySystemComponent()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& StatusTag, int32 SpellPoints,
                                                     bool& bShouldEnableSpendPointButton, bool& bShouldEnableEquipButton)
{
	const FARPGGameplayTags GameplayTags = FARPGGameplayTags::Get();
	bShouldEnableEquipButton = false;
	bShouldEnableSpendPointButton = false;
	if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		bShouldEnableSpendPointButton = SpellPoints > 0;
	}
	else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		bShouldEnableSpendPointButton = SpellPoints > 0;
	}
	else if (StatusTag.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		bShouldEnableSpendPointButton = SpellPoints > 0;
	}
}
