// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyPad.h"
#include "GameJam2Character.h"

// Sets default values
AKeyPad::AKeyPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KeypadMesh = CreateDefaultSubobject<UStaticMeshComponent>("KeypadMesh");
	KeypadMesh->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UStaticMeshComponent>("CollisionsBox");
	CollisionBox->SetupAttachment(KeypadMesh);
	CollisionBox->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	//Register OnBeginOverlap function as an OnActorBeginOverlap delegate
	OnActorBeginOverlap.AddDynamic(this, &AKeyPad::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AKeyPad::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKeyPad::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player")) {
		AGameJam2Character* Player = Cast<AGameJam2Character>(OtherActor);
		Player->SetCurrentKeypad(this);
	}
}

void AKeyPad::OnEndOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player")) {
		AGameJam2Character* Player = Cast<AGameJam2Character>(OtherActor);
		Player->ClearCurrentKeypad();
	}
}

// Called every frame
void AKeyPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

