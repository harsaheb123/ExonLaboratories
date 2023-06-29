// Fill out your copyright notice in the Description page of Project Settings.


#include "STrackerBot.h"
#include "GameJam2Character.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/StaticMeshComponent.h"
#include "GameJam2Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"



// Sets default values
ASTrackerBot::ASTrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(200);
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//SphereComp->SetCollisionResponseToChannels(ECR_Ignore);
	//SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	bUseVelocityChange = false;
	MovementForce = 1000;
	RequiredDistanceToTarget = 100;

	ExplosionDamage = 100;
	ExplosionRadius = 400;

	bInvincible = false;
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	//Initial move to
	NextPathPoint = GetNextPathPoint();
}

void ASTrackerBot::HandleTakeDamage(int damageTaken)
{
	//Explode on death

	//Pulse Material on hit





	currentHealth -= damageTaken;


	if (currentHealth <= 0) {
		SelfDestruct();
	}
}

void ASTrackerBot::SelfDestruct() {

	if (!bInvincible) {
		if (bExploded) {
			return;
		}
		bExploded = true;


		//Apply the damage
		//UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

		//UParticleSystemComponent* UGameplayStatics::SpawnEmitterAtLocation(UObject * WorldContextObject, UParticleSystem * EmitterTemplate, FVector SpawnLocation, FRotator SpawnRotation, bool bAutoDestroy);


		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation());

		//DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 2.0f, 0, 1.0f);


		this->Destroy();
	}

}

void ASTrackerBot::DamageSelf() {
	currentHealth -= 20;
}

FVector ASTrackerBot::GetNextPathPoint() {

	ACharacter* MyPlayer = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), MyPlayer);


	if (NavPath->PathPoints.Num() > 1) {
		//return next point in path
	
		return NavPath->PathPoints[1];
	}
	//Failed to find actor
	return GetActorLocation();
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

	if (DistanceToTarget <= RequiredDistanceToTarget) {
		//
		NextPathPoint = GetNextPathPoint();

	}

	else {

		//Keep moving towards next target
		FVector ForceDirection = NextPathPoint - GetActorLocation();
		ForceDirection.Normalize();
		ForceDirection *= MovementForce;
		MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);

	}


}


void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor) {

	AGameJam2Character* PlayerPawn = Cast<AGameJam2Character>(OtherActor);
	if (bStartedSelfDestruction == false) {
		if (PlayerPawn) {
	
			//Overlapped with player

			//GetWorldTimerManager().SetTimer(selfDamage, this, &ASTrackerBot::DamageSelf, 0.5f, true, 0.0f);
			SelfDestruct();
			bStartedSelfDestruction = true;
		}
	}
}