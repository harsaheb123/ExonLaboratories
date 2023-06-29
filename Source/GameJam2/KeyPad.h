// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyPad.generated.h"

UCLASS()
class GAMEJAM2_API AKeyPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeyPad();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Codepad, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* KeypadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Codepad, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Codepad, meta = (AllowPrivateAccess = "true"))
		int code;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Codepad, meta = (AllowPrivateAccess = "true"))
		bool bIsEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Codepad, meta = (AllowPrivateAccess = "true"))
		bool bCorrect = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
