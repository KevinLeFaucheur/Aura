


#include "UI/WidgetController/OverlayWidgetController.h"

#include "ARPGGameplayTags.h"
#include "AbilitySystem/ARPGAbilitySystemComponent.h"
#include "AbilitySystem/ARPGAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/CharacterPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{	
	OnHealthChanged.Broadcast(GetARPGAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetARPGAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetARPGAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetARPGAttributeSet()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetCharacterPlayerState()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetCharacterPlayerState()->OnLevelChangedDelegate.AddLambda(
		[this] (int32 NewLevel)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
	});
		
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetARPGAttributeSet()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) { OnHealthChanged.Broadcast(Data.NewValue); }
	);	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetARPGAttributeSet()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ OnManaChanged.Broadcast(Data.NewValue); }
	);	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetARPGAttributeSet()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) { OnMaxHealthChanged.Broadcast(Data.NewValue); }
	);	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetARPGAttributeSet()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) { OnMaxManaChanged.Broadcast(Data.NewValue); }
	);

	if (GetARPGAbilitySystemComponent())
	{
		GetARPGAbilitySystemComponent()->AbilityEquippedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if(GetARPGAbilitySystemComponent()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetARPGAbilitySystemComponent()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
		
		GetARPGAbilitySystemComponent()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for(const FGameplayTag& Tag : AssetTags)
				{
					// For example, Tage = Message.HealthPotion
					//  "Message.HealthPotion".MatchesTag(Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));

					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						OnMessageWidgetRowDelegate.Broadcast(*Row);	
					}
				}
			}
		);
	}
	
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetCharacterPlayerState()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo, Fill in PlayerState BP "));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if(Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelUpRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelUpRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FARPGGameplayTags& GameplayTags = FARPGGameplayTags::Get();
	
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
}
