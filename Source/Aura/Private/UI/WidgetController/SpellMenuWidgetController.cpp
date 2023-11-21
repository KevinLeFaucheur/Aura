


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
				FString Description;
				FString NextLevelDescription;
				GetARPGAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
				OnSpellGlobeSelectedDelegate.Broadcast(bEnableSpendPointButton, bEnableEquipButton, Description, NextLevelDescription);
			}
			
			if(AbilityInfo)
			{
				FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	GetARPGAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
	
	GetCharacterPlayerState()->OnSpellPointsChangedDelegate.AddLambda(
		[this] (int32 SpellsPoints)
		{
			SpellPointsChanged.Broadcast(SpellsPoints);
			CurrentSpellPoints = SpellsPoints;
			
			bool bEnableSpendPointButton = false;
			bool bEnableEquipButton = false;
			ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPointButton, bEnableEquipButton);
			FString Description;
			FString NextLevelDescription;
			GetARPGAbilitySystemComponent()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
			OnSpellGlobeSelectedDelegate.Broadcast(bEnableSpendPointButton, bEnableEquipButton, Description, NextLevelDescription);
		});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if(bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType =  AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityTypeTag;
		StopWaitingForEquipSelectionDelegate.Broadcast(SelectedAbilityType);		
		bWaitingForEquipSelection = false;
	}
	
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
	FString Description;
	FString NextLevelDescription;
	GetARPGAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	OnSpellGlobeSelectedDelegate.Broadcast(bEnableSpendPointButton, bEnableEquipButton, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpellGlobeDeselect()
{
	if(bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType =  AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityTypeTag;
		StopWaitingForEquipSelectionDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	SelectedAbility.Ability = FARPGGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FARPGGameplayTags::Get().Abilities_Status_Locked;
	OnSpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if(GetARPGAbilitySystemComponent())
	{
		GetARPGAbilitySystemComponent()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag& AbilityTypeTag = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityTypeTag;
	WaitForEquipSelectionDelegate.Broadcast(AbilityTypeTag);
	bWaitingForEquipSelection = true;

	const FGameplayTag SelectedStatus = GetARPGAbilitySystemComponent()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if(SelectedStatus.MatchesTagExact(FARPGGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetARPGAbilitySystemComponent()->GetInputTagFromAbilityTag(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if(!bWaitingForEquipSelection) return;
	// check selected ability against the slot ability type (offensive / passive)

	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityTypeTag;
	if(!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	GetARPGAbilitySystemComponent()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	const FARPGGameplayTags& GameplayTags = FARPGGameplayTags::Get();
	
	bWaitingForEquipSelection = false;
	FBaseAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// Broadcast empty info if previous slot is a valid slot, only if equipping an already equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);
	
	FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitingForEquipSelectionDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityTypeTag);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	SpellGlobeDeselect();
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
