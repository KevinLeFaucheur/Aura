


#include "AbilitySystem/Abilities/FireBolt.h"

#include "Kismet/KismetSystemLibrary.h"

void UFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
                                 bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if(bOverridePitch) Rotation.Pitch = PitchOverride;

	const FVector Forward = Rotation.Vector();
	const FVector LeftOfSpread = Forward.RotateAngleAxis(- ProjectileSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.f, FVector::UpVector);
	//NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	if(NumProjectiles > 1)
	{
		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			const FVector Start = SocketLocation + FVector(0, 0, 5);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Start, Start + Direction * 75.f, 5, FLinearColor::Yellow, 60.f, 1.5f);
		}
	}
	else
	{
		const FVector Start = SocketLocation + FVector(0,0,5);
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			Start,
			Start + Forward * 75.f,
			1,
			FLinearColor::Red,
			120,
			1);
		
	}

	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation +Forward * 100.f, 5, FLinearColor::White, 60.f, 1.5f);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.f, 5, FLinearColor::Blue, 60.f, 1.5f);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.f, 5, FLinearColor::Blue, 60.f, 1.5f);
}

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
