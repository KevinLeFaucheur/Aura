


#include "Actor/CharacterEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/ARPGAttributeSet.h"
#include "Components/SphereComponent.h"

ACharacterEffectActor::ACharacterEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());

}

void ACharacterEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: Change to Gameplay Effect
	if(IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UARPGAttributeSet* ARPGAttributeSet = Cast<UARPGAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UARPGAttributeSet::StaticClass()));
		UARPGAttributeSet* MutableARPGAttributeSet = const_cast<UARPGAttributeSet*>(ARPGAttributeSet);
		MutableARPGAttributeSet->SetHealth(ARPGAttributeSet->GetHealth() + 25.f);
		MutableARPGAttributeSet->SetMana(ARPGAttributeSet->GetMana() + 5.f);
		Destroy();
	}
}

void ACharacterEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ACharacterEffectActor::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACharacterEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ACharacterEffectActor::EndOverlap);
}

