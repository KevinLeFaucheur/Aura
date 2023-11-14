


#include "UI/WidgetController/WidgetController.h"

#include "AbilitySystem/ARPGAbilitySystemComponent.h"
#include "AbilitySystem/ARPGAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/CharacterPlayerController.h"
#include "Player/CharacterPlayerState.h"

void UWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UWidgetController::BroadcastInitialValues()
{
}

void UWidgetController::BindCallbacksToDependencies()
{
}

void UWidgetController::BroadcastAbilityInfo()
{
	if(!GetARPGAbilitySystemComponent()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FBaseAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(ARPGAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = ARPGAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		}
	);
	GetARPGAbilitySystemComponent()->ForEachAbility(BroadcastDelegate);
}

ACharacterPlayerController* UWidgetController::GetCharacterPlayerController()
{
	if(CharacterPlayerController == nullptr)
	{
		CharacterPlayerController = Cast<ACharacterPlayerController>(PlayerController);
	}
	return  CharacterPlayerController;
}

ACharacterPlayerState* UWidgetController::GetCharacterPlayerState()
{
	if(CharacterPlayerState == nullptr)
	{
		CharacterPlayerState = Cast<ACharacterPlayerState>(PlayerState);
	}
	return  CharacterPlayerState;	
}

UARPGAbilitySystemComponent* UWidgetController::GetARPGAbilitySystemComponent()
{
	if(ARPGAbilitySystemComponent == nullptr)
	{
		ARPGAbilitySystemComponent = Cast<UARPGAbilitySystemComponent>(AbilitySystemComponent);
	}
	return  ARPGAbilitySystemComponent;	
}

UARPGAttributeSet* UWidgetController::GetARPGAttributeSet()
{
	if(ARPGAttributeSet == nullptr)
	{
		ARPGAttributeSet = Cast<UARPGAttributeSet>(AttributeSet);
	}
	return  ARPGAttributeSet;	
}
