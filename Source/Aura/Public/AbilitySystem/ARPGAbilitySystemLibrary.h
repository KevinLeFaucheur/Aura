

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemTypes.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ARPGAbilitySystemLibrary.generated.h"

class UAbilityInfo;
class ACharacterHUD;
struct FWidgetControllerParams;
struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UARPGAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static  bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetControllerParams, ACharacterHUD*& OutCharacterHUD);
	
	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static  UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static  UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf = "WorldContextObject"))
	static  USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|AbilityInfo")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|DamageEffects")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& Params);

	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  void SetIsBlockedHit(UPARAM(Ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  void SetIsCriticalHit(UPARAM(Ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  void SetIsSuccessfulDebuff(UPARAM(Ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  void SetDebuffDamage(UPARAM(Ref) FGameplayEffectContextHandle& EffectContextHandle, float InDamage);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  void SetDebuffDuration(UPARAM(Ref) FGameplayEffectContextHandle& EffectContextHandle, float InDuration);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  void SetDebuffFrequency(UPARAM(Ref) FGameplayEffectContextHandle& EffectContextHandle, float InFrequency);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  void SetDamageType(UPARAM(Ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  void SetDeathImpulse(UPARAM(Ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|GameplayEffects")
	static  void SetKnockbackForce(UPARAM(Ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackForce);

	UFUNCTION(BlueprintCallable, Category="ARPGAbilitySystemLibrary|GameplayMechanics")
	static  void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category="ARPGAbilitySystemLibrary|GameplayMechanics")
	static bool IsHostile(AActor* FirstActor, AActor* SecondActor);

	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
	
};



