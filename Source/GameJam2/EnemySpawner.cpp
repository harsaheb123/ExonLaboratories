// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "AICharacter.h"
#include "STrackerBot.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Register OnBeginOverlap function as an OnActorBeginOverlap delegate
	OnActorBeginOverlap.AddDynamic(this, &AEnemySpawner::OnBeginOverlap);

	//RoomEntrance = CreateDefaultSubobject<UMeshComponent>(TEXT("RoomEnterance"));
	//RoomEntrance->SetupAttachment(RootComponent);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComponent);
	SpawnPoint->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	RoomEntrance = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoomEnterance"));
	RoomEntrance->SetupAttachment(SpawnPoint);
	RoomEntrance->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	PatrolToLocation = CreateDefaultSubobject<USceneComponent>(TEXT("PatrolToLocation"));
	PatrolToLocation->SetupAttachment(SpawnPoint);
	PatrolToLocation->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	EnemyType = 0;
	WeaponType = 0;
	bIsInvincible = false;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void AEnemySpawner::ResetCounterTimer()
{
	SecondsAfterStart++;
	bCountTimerOnCooldown = false;
	GetWorldTimerManager().ClearTimer(CounterTimeHandle);
}

void AEnemySpawner::ResetTimeBeforeSpawnEnemy1Timer()
{
}

void AEnemySpawner::ResetEnemy1RepeatTimer()
{
	bSpawnOnCooldown = false;
	GetWorldTimerManager().ClearTimer(RepeatSpawnEnemy1Handle);
	AAICharacter* spawnedActor = Cast<AAICharacter>(GetWorld()->SpawnActor<AActor>(Enemy1, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation(), SpawnParams));
	if (spawnedActor->IsValidLowLevelFast()) {
		spawnedActor->maxHealth = Enemy1Health;
		spawnedActor->currentHealth = Enemy1Health;
		spawnedActor->PatrolLocation = PatrolToLocation->GetComponentLocation();
		spawnedActor->bPatroling = bPatrol;
		spawnedActor->bPatrolingReturn = !bStopWhenReachLocation;
	}
	NumberOfRepeatsEnemy1--;
	if (NumberOfRepeatsEnemy1 <= 0) {
		bRepeatSpawnEnemy1 = false;
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bCountTimerOnCooldown && bSpawnEnemies) { // if the spawner has been triggered then start the counter
		bCountTimerOnCooldown = true;
		GetWorld()->GetTimerManager().SetTimer(CounterTimeHandle, this, &AEnemySpawner::ResetCounterTimer, 1, true);
	}
	else if (bSpawnEnemies && SecondsAfterStart == TimeBeforeInitialSpawnEnemy1 && bDoOnce) { //once the counter has reached the assgned cooldown
		// make sure that this is only done once otherwise the spawner would constantly spawn enemies over the assigned TimeBeforeInitialSpawnEnemy1
		bDoOnce = false;
		if (EnemyType == 0) {
			AAICharacter* spawnedActor = Cast<AAICharacter>(GetWorld()->SpawnActor<AActor>(Enemy1, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation(), SpawnParams));
			if (spawnedActor->IsValidLowLevelFast()) {
				spawnedActor->maxHealth = Enemy1Health;
				spawnedActor->currentHealth = Enemy1Health;
				spawnedActor->PatrolLocation = PatrolToLocation->GetComponentLocation();
				spawnedActor->bPatroling = bPatrol;
				spawnedActor->bPatrolingReturn = !bStopWhenReachLocation;
				spawnedActor->CurrentWeapon = WeaponType;
			}
		}
		if (EnemyType == 1) {
			//ASTrackerBot* spawnedTracker = Cast<ASTrackerBot>(GetWorld()->SpawnActor<APawn>(Enemy1, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation(), SpawnParams));
			//GetWorld()->SpawnActor<ASTrackerBot>(SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation(), SpawnParams);
			ASTrackerBot* spawnedActor = Cast<ASTrackerBot>(GetWorld()->SpawnActor<AActor>(Enemy1, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation(), SpawnParams));
			//spawnedActor->bInvincible = bIsInvincible;
		}
		bStartRepeatSpawn = true;
	}
	else if (bSpawnEnemies && bRepeatSpawnEnemy1 && bStartRepeatSpawn && NumberOfRepeatsEnemy1 > 0 && !bSpawnOnCooldown) { // if the enemy spawn needs to be repeated and it is time to spawn the repeated enemy
		bSpawnOnCooldown = true;
		GetWorld()->GetTimerManager().SetTimer(RepeatSpawnEnemy1Handle, this, &AEnemySpawner::ResetEnemy1RepeatTimer, IntervalBetweenRepeatSpawnsEnemy1, false);
	}
	//else if (bStartRepeatSpawn == true && bRepeatSpawnEnemy1 == false) {
	//	Destroy();
	//}
}

void AEnemySpawner::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player") && !bSpawnEnemies) {
		bSpawnEnemies = true;
	}
}

