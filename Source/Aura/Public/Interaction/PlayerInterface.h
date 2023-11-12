

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level) const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 Level) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InLevel) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePointsReward(int32 InAttributePoints) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPointsReward(int32 InSpellPoints) const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP) const;

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
};
