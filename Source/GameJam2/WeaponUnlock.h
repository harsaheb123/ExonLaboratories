// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponUnlock.generated.h"

UCLASS()
class GAMEJAM2_API AWeaponUnlock : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponUnlock();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* CollisionMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);*/

	UFUNCTION()
		void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
	UPROPERTY(VisibleAnywhere)
		bool bMovingUp = true;
};
