// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponUnlock.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/SphereComponent.h"
#include "GameJam2Character.h"

// Sets default values
AWeaponUnlock::AWeaponUnlock()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Weapon Type");

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skel Mesh Comp"));
	MeshComponent->SetupAttachment(RootComponent);

	CollisionMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Collision Mesh Comp"));
	CollisionMeshComponent->SetupAttachment(RootComponent);
	CollisionMeshComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	OnActorBeginOverlap.AddDynamic(this, &AWeaponUnlock::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AWeaponUnlock::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeaponUnlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = RootComponent->GetComponentLocation();
	FRotator NewRotation = RootComponent->GetComponentRotation();

	float DelatRotation = DeltaTime * 40;
	NewRotation.Yaw += DelatRotation;
	NewRotation.Pitch += DelatRotation;
	RootComponent->SetRelativeRotation(NewRotation);
}

void AWeaponUnlock::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{

	if (OtherActor != this && OtherActor->ActorHasTag("Player"))
	{
		AGameJam2Character* Player = Cast<AGameJam2Character>(OtherActor);
		if (Tags[0] == "Pistol")
		{
			Player->CurrentPistolAmmo = Player->CurrentPistolMaxAmmo;
			Player->CurrentAmmoInPistolClip = Player->CurrentPistolClipSize;
			Destroy();
		}
		if (Tags[0] == "Shotgun")
		{
			if (!Player->bHasShotgun)
			{
				Player->bHasShotgun = true;
				
			}
			else
			{
				Player->CurrentShotgunAmmo = Player->CurrentShotgunMaxAmmo;
				Player->CurrentAmmoInShotgunClip = Player->CurrentShotgunClipSize;
			}
			Destroy();
		}
		else if (Tags[0] == "AK")
		{
			
			if (!Player->bHasAK)
			{
				Player->bHasAK = true;

			}
			else
			{
				Player->CurrentAKAmmo = Player->CurrentAKMaxAmmo;
				Player->CurrentAmmoInAKClip = Player->CurrentAKClipSize;
			}
			Destroy();
		}
		else if (Tags[0] == "SMG")
		{
			if (!Player->bHasSMG)
			{
				Player->bHasSMG = true;
			}
			else
			{
				Player->CurrentSMGAmmo = Player->CurrentSMGMaxAmmo;
				Player->CurrentAmmoInSMGClip = Player->CurrentSMGClipSize;
			}
			Destroy();
		}
		else if (Tags[0] == "RPG")
		{
			Player->bHasRPG = true;
			if (!Player->bHasRPG)
			{
				Player->bHasRPG = true;
			}
			else
			{
				Player->CurrentRPGAmmo = Player->CurrentRPGMaxAmmo;
				Player->CurrentAmmoInRPGClip = Player->CurrentRPGClipSize;
			}
			Destroy();
		}
	}
}

/*void AWeaponUnlock::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Collsion"));
	if (OtherActor != this && OtherActor->IsA(AGameJam2Character::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Collision with player"));
		AGameJam2Character* Player = Cast<AGameJam2Character>(OtherActor);
		Player->CurrentHealth -= 50;
	}
}*/

