


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FBaseAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FBaseAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}
	if(bLogNotFound)
	{
		UE_LOG(LogARPG, Error, TEXT("Can't find info for Ability Tag [%s]' on Ability Info [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	return FBaseAbilityInfo();
}
