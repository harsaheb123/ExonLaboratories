// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class GAMEJAM2_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Set up Spawn Points

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnPoints, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* RoomEntrance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnPoints, meta = (AllowPrivateAccess = "true"))
		USceneComponent* SpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpawnPoints, meta = (AllowPrivateAccess = "true"))
		USceneComponent* PatrolToLocation;

	//Set up which enemies to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemies, meta = (AllowPrivateAccess = "true"))
		UClass* Enemy1;

	//Set up counter timer to count the seconds after this room has been activated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Counter, meta = (AllowPrivateAccess = "true"))
		int SecondsAfterStart = 0;
	FTimerHandle CounterTimeHandle;
	bool bCountTimerOnCooldown = false;
	void ResetCounterTimer();

	//Set up the times before each enemy is spawned when the player enters the room
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = InitialTimeBeforeSpawn, meta = (AllowPrivateAccess = "true"))
		int TimeBeforeInitialSpawnEnemy1;
	FTimerHandle TimeBeforeSpawnEnemy1Handle;
	bool bSpawnOnCooldown;
	void ResetTimeBeforeSpawnEnemy1Timer();

	//Repeat enemy spawns?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RepeatEnemySpawns, meta = (AllowPrivateAccess = "true"))
		bool bRepeatSpawnEnemy1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RepeatEnemySpawns, meta = (AllowPrivateAccess = "true"))
		bool bStartRepeatSpawn = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RepeatEnemySpawns, meta = (AllowPrivateAccess = "true"))
		int IntervalBetweenRepeatSpawnsEnemy1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RepeatEnemySpawns, meta = (AllowPrivateAccess = "true"))
		int NumberOfRepeatsEnemy1;
	FTimerHandle RepeatSpawnEnemy1Handle;
	void ResetEnemy1RepeatTimer();

	//Patroling
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patroling, meta = (AllowPrivateAccess = "true"))
		bool bPatrol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patroling, meta = (AllowPrivateAccess = "true"))
		bool bStopWhenReachLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Patroling, meta = (AllowPrivateAccess = "true"))
		float speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		int WeaponType;
	//Set Up Weapons and health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyStatistics, meta = (AllowPrivateAccess = "true"))
		int Enemy1Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EnemyStatistics, meta = (AllowPrivateAccess = "true"))
		int Enemy1weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RepeatEnemySpawns, meta = (AllowPrivateAccess = "true"))
		int EnemyType;

	bool bDoOnce = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool bSpawnEnemies = false;


	/*Is enemy invincible? Added for final level, dont want the ball to explode in the background*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RepeatEnemySpawns, meta = (AllowPrivateAccess = "true"))
		bool bIsInvincible = false;

	FActorSpawnParameters SpawnParams;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//The delegate function for handling an overlap event
	UFUNCTION()
		void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

};
