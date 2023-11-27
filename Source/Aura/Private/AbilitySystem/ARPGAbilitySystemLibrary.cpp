


#include "AbilitySystem/ARPGAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemTypes.h"
#include "ARPGGameplayTags.h"
#include "Game/ARPGGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CharacterPlayerState.h"
#include "UI/HUD/CharacterHUD.h"
#include "UI/WidgetController/WidgetController.h"

bool UARPGAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetControllerParams, ACharacterHUD*& OutCharacterHUD)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutCharacterHUD = Cast<ACharacterHUD>(PlayerController->GetHUD());
		if (OutCharacterHUD)
		{
			ACharacterPlayerState* PlayerState = PlayerController->GetPlayerState<ACharacterPlayerState>();
			UAbilitySystemComponent* ASC = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AS = PlayerState->GetAttributeSet();
			OutWidgetControllerParams.AttributeSet = AS;
			OutWidgetControllerParams.PlayerController = PlayerController;
			OutWidgetControllerParams.PlayerState = PlayerState;
			OutWidgetControllerParams.AbilitySystemComponent = ASC;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UARPGAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ACharacterHUD* CharacterHUD = nullptr;
	if(MakeWidgetControllerParams(WorldContextObject, WCParams, CharacterHUD))
	{
			return CharacterHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UARPGAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ACharacterHUD* CharacterHUD = nullptr;
	if(MakeWidgetControllerParams(WorldContextObject, WCParams, CharacterHUD))
	{
		return CharacterHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UARPGAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ACharacterHUD* CharacterHUD = nullptr;
	if(MakeWidgetControllerParams(WorldContextObject, WCParams, CharacterHUD))
	{
		return CharacterHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
}

void UARPGAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{	
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
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

void UARPGAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{	
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1.f);
		ASC->GiveAbility(AbilitySpec);
	}
	
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if(ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

int32 UARPGAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

UCharacterClassInfo* UARPGAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AARPGGameModeBase* ARPGGameMode = Cast<AARPGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(ARPGGameMode == nullptr) return nullptr;

	return ARPGGameMode->CharacterClassInfo;
}

UAbilityInfo* UARPGAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AARPGGameModeBase* ARPGGameMode = Cast<AARPGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(ARPGGameMode == nullptr) return nullptr;

	return ARPGGameMode->AbilityInfo;	
}

FGameplayEffectContextHandle UARPGAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& Params)
{
	const FARPGGameplayTags GameplayTags = FARPGGameplayTags::Get();
	const AActor* SourceAvatarActor = Params.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContextHandle = Params.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, Params.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, Params.KnockbackForce);
	const FGameplayEffectSpecHandle SpecHandle = Params.SourceAbilitySystemComponent->MakeOutgoingSpec(Params.DamageGameplayEffectClass, Params.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Params.DamageType, Params.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, Params.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, Params.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, Params.DebuffFrequency);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, Params.DebuffDuration);

	Params.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data); // Doesn't have to call  Data.Get() when de-referenced

	return EffectContextHandle;
}

bool UARPGAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FARPGGameplayEffectContext* ARPGEffectContext = static_cast<const FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->IsBlockedHit();
	}
	return false;
}

bool UARPGAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FARPGGameplayEffectContext* ARPGEffectContext = static_cast<const FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->IsCriticalHit();
	}
	return false;
}

bool UARPGAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FARPGGameplayEffectContext* ARPGEffectContext = static_cast<const FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->IsSuccessfulDebuff();
	}
	return false;	
}

float UARPGAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FARPGGameplayEffectContext* ARPGEffectContext = static_cast<const FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UARPGAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FARPGGameplayEffectContext* ARPGEffectContext = static_cast<const FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UARPGAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FARPGGameplayEffectContext* ARPGEffectContext = static_cast<const FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UARPGAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FARPGGameplayEffectContext* ARPGEffectContext = static_cast<const FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if(ARPGEffectContext->GetDamageType().IsValid())
		{
			return *ARPGEffectContext->GetDamageType().Get();
		}
	}
	return FGameplayTag();
}

FVector UARPGAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FARPGGameplayEffectContext* ARPGEffectContext = static_cast<const FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UARPGAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FARPGGameplayEffectContext* ARPGEffectContext = static_cast<const FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

void UARPGAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FARPGGameplayEffectContext* ARPGEffectContext = static_cast<FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UARPGAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if (FARPGGameplayEffectContext* ARPGEffectContext = static_cast<FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UARPGAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsSuccessfulDebuff)
{
	if (FARPGGameplayEffectContext* ARPGEffectContext = static_cast<FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}	
}

void UARPGAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FARPGGameplayEffectContext* ARPGEffectContext = static_cast<FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->SetDebuffDamage(InDamage);
	}	
}

void UARPGAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FARPGGameplayEffectContext* ARPGEffectContext = static_cast<FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->SetDebuffDuration(InDuration);
	}	
}

void UARPGAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FARPGGameplayEffectContext* ARPGEffectContext = static_cast<FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->SetDebuffFrequency(InFrequency);
	}	
}

void UARPGAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FARPGGameplayEffectContext* ARPGEffectContext = static_cast<FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		return ARPGEffectContext->SetDamageType(DamageType);
	}	
}

void UARPGAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InDeathImpulse)
{
	if (FARPGGameplayEffectContext* ARPGEffectContext = static_cast<FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->SetDeathImpulse(InDeathImpulse);
	}	
}

void UARPGAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InKnockbackForce)
{
	if (FARPGGameplayEffectContext* ARPGEffectContext = static_cast<FARPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return ARPGEffectContext->SetKnockbackForce(InKnockbackForce);
	}	
}

void UARPGAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                           TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                           const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	TArray<FOverlapResult> Overlaps;
	if(const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(
			Overlaps,
			SphereOrigin,
			FQuat::Identity,
			FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius),
			SphereParams
		);
		for(FOverlapResult& Overlap : Overlaps)
		{
			if(Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UARPGAbilitySystemLibrary::IsHostile(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayer = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemy = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	return !(bBothArePlayer || bBothAreEnemy);
}
