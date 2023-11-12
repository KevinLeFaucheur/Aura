


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP)
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		// LevelUpInformation[0 ]is placeholder 
		// LevelUpInformation[1] = Level 1 Information
		if(LevelUpInformation.Num() <= Level) return Level;
		if(XP >= LevelUpInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
