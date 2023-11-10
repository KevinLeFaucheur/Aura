


#include "AbilitySystem/Abilities/BaseSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UBaseSummonAbility::GetSpawnLocations()
{	
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(- SpawnSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		const FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		SpawnLocations.Add(ChosenSpawnLocation);
		
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + Direction * MaxSpawnDistance, 4.f, FLinearColor::Yellow, 3.f);
		DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 15.f, 12, FColor::Red, false, 3.f);
	}

	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + RightOfSpread * MaxSpawnDistance, 4.f, FLinearColor::Green, 3.f);
	DrawDebugSphere(GetWorld(), Location + RightOfSpread * MinSpawnDistance, 8.f, 12, FColor::Green, false, 3.f);
	DrawDebugSphere(GetWorld(), Location + RightOfSpread * MaxSpawnDistance, 8.f, 12, FColor::Green, false, 3.f);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxSpawnDistance, 4.f, FLinearColor::Blue, 3.f);
	
	return SpawnLocations;
}
