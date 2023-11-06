


#include "AbilitySystem/ARPGAbilitySystemLibrary.h"

#include "Game/ARPGGameModeBase.h"
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

void UARPGAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AARPGGameModeBase* ARPGGameMode = Cast<AARPGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(ARPGGameMode == nullptr) return;
	
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = ARPGGameMode->CharacterClassInfo;
	//UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	 FGameplayEffectContextHandle PrimaryAttributesContextHandle =  ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle SecondaryAttributesContextHandle =  ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle VitalAttributesContextHandle =  ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UARPGAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	AARPGGameModeBase* ARPGGameMode = Cast<AARPGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(ARPGGameMode == nullptr) return;
	
	UCharacterClassInfo* CharacterClassInfo = ARPGGameMode->CharacterClassInfo;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1.f);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UARPGAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AARPGGameModeBase* ARPGGameMode = Cast<AARPGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(ARPGGameMode == nullptr) return nullptr;

	return ARPGGameMode->CharacterClassInfo;
}
