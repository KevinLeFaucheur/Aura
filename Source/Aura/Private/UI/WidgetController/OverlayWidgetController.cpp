


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/ARPGAbilitySystemComponent.h"
#include "AbilitySystem/ARPGAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/CharacterPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UARPGAttributeSet* ARPGAttributeSet = CastChecked<UARPGAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(ARPGAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(ARPGAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(ARPGAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(ARPGAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	ACharacterPlayerState* CharacterPlayerState = CastChecked<ACharacterPlayerState>(PlayerState);
	CharacterPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	CharacterPlayerState->OnLevelChangedDelegate.AddLambda(
		[this] (int32 NewLevel)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
	});
	
	const UARPGAttributeSet* ARPGAttributeSet = CastChecked<UARPGAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ARPGAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) { OnHealthChanged.Broadcast(Data.NewValue); }
	);	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ARPGAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data){ OnManaChanged.Broadcast(Data.NewValue); }
	);	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ARPGAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) { OnMaxHealthChanged.Broadcast(Data.NewValue); }
	);	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ARPGAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) { OnMaxManaChanged.Broadcast(Data.NewValue); }
	);

	if (UARPGAbilitySystemComponent* ARPGAbilitySystemComponent = Cast<UARPGAbilitySystemComponent>(AbilitySystemComponent))
	{
		if(ARPGAbilitySystemComponent->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(ARPGAbilitySystemComponent);
		}
		else
		{
			ARPGAbilitySystemComponent->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}
		
		ARPGAbilitySystemComponent->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::OnInitializeStartupAbilities(UARPGAbilitySystemComponent* ARPGAbilitySystemComponent)
{
	//TODO: Get info about all given abilities and broadcast to widgets
	if(!ARPGAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this, ARPGAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
		{
			FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(ARPGAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = ARPGAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	ARPGAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const ACharacterPlayerState* CharacterPlayerState = CastChecked<ACharacterPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = CharacterPlayerState->LevelUpInfo;
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
