


#include "AbilitySystem/Abilities/FireBolt.h"

#include "ARPGGameplayTags.h"

FString UFireBolt::GetDescription(int32 Level)
{
	const FString NumOfProjectiles =  Level == 1 ? FString("a bolt") : FString::Printf(TEXT("%d bolts"), FMath::Min(NumProjectiles, Level));
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	
	return FString::Printf(TEXT(
		"<Title>FIRE BOLT</>\n\n"
		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		//Description
		"<Default>Launches %s of fire, exploding on impact and dealing: </>"
		// Damage
		"<Damage>%d</><Default> fire damage with a chance to burn.</>\n\n"),
		Level,
		ManaCost,
		Cooldown,
		*NumOfProjectiles,
		ScaledDamage);
}

FString UFireBolt::GetNextLevelDescription(int32 Level)
{
	const FString NumOfProjectiles =  Level == 1 ? FString("a bolt") : FString::Printf(TEXT("%d bolts"), FMath::Min(NumProjectiles, Level));
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL</>\n\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>\n"		
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		"<Default>Launches %s of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn.</>\n\n"),
		Level,
		ManaCost,
		Cooldown,
		*NumOfProjectiles,
		ScaledDamage);
}