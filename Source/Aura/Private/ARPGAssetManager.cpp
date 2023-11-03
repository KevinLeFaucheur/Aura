


#include "ARPGAssetManager.h"
#include "ARPGGameplayTags.h"
#include "AbilitySystemGlobals.h"

UARPGAssetManager& UARPGAssetManager::Get()
{
	check(GEngine);
	UARPGAssetManager* ARPGAssetManager = Cast<UARPGAssetManager>(GEngine->AssetManager);
	return *ARPGAssetManager;
}

void UARPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FARPGGameplayTags::InitializeNativeGameplayTags();

	// This is required to use Target Data
	UAbilitySystemGlobals::Get().InitGlobalData();
}
