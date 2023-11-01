


#include "AbilitySystem/ARPGAbilitySystemLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/CharacterPlayerState.h"
#include "UI/HUD/CharacterHUD.h"
#include "UI/WidgetController/WidgetController.h"

UOverlayWidgetController* UARPGAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ACharacterHUD* CharacterHUD = Cast<ACharacterHUD>(PlayerController->GetHUD()))
		{
			ACharacterPlayerState* PlayerState = PlayerController->GetPlayerState<ACharacterPlayerState>();
			UAbilitySystemComponent* ASC = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AS = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, ASC, AS);
			return CharacterHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UARPGAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ACharacterHUD* CharacterHUD = Cast<ACharacterHUD>(PlayerController->GetHUD()))
		{
			ACharacterPlayerState* PlayerState = PlayerController->GetPlayerState<ACharacterPlayerState>();
			UAbilitySystemComponent* ASC = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AS = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PlayerController, PlayerState, ASC, AS);
			return CharacterHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
