// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "GameJam2Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "GameJam2Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"


// Sets default values
AAICharacter::AAICharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialise pawn sensing component

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	//SetPeripheral vision to 90 **WILL MOST LIKELY CHANGE FOR CONE VISION
	PawnSensingComp->SetPeripheralVisionAngle(90.f);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(RootComponent);
	MuzzleLocation->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	MuzzleLocation1 = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation1"));
	MuzzleLocation1->SetupAttachment(RootComponent);
	MuzzleLocation1->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	MuzzleLocation2 = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation2"));
	MuzzleLocation2->SetupAttachment(RootComponent);
	MuzzleLocation2->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	MuzzleLocation3 = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation3"));
	MuzzleLocation3->SetupAttachment(RootComponent);
	MuzzleLocation3->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	MuzzleLocation4 = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation4"));
	MuzzleLocation4->SetupAttachment(RootComponent);
	MuzzleLocation4->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	//Register function that is going to fire when character sees pawn

	if (PawnSensingComp) {
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AAICharacter::OnSeePlayer);
	}

}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCanSeePlayer && !bIsDead) {
		AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
		FRotator RotationToLookAt = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Player->GetActorLocation());
		this->SetActorRotation(RotationToLookAt);
		Fire();
	}
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAICharacter::OnSeePlayer(APawn* pawn)
{
	bCanSeePlayer = true;
	//AMyAIController* AIController = Cast<AMyAIController>(GetController());

	//if (AIController) {
	//	GLog->Log("Hello There");
	//}

	//GetWorldTimerManager().SetTimer(ReloadTimerHandle, this, &AAICharacter::Fire, 0.5f, true, 1.0f);
	//AIController->SetSeenTarget(pawn);
}


void AAICharacter::Fire() {
	if (!bIsDead) {
		if (CurrentWeapon == 0)
		{
			if (bReloading == false && CurrentPistolAmmo > 0 && CurrentAmmoInPistolClip > 0)
			{
				if (CurrentFiringMode == 0)
				{
					UWorld* World = GetWorld();
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					if (CurrentProjectileClass->IsValidLowLevelFast() && MuzzleLocation->IsValidLowLevelFast())
					{
						FHitResult Hit;
						UClass* GenerateBPBullet = Cast<UClass>(CurrentProjectileClass);
						World->SpawnActor<AActor>(GenerateBPBullet, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation(), SpawnParams);
						if (GlockShootSound->IsValidLowLevelFast())
						{
							UGameplayStatics::PlaySoundAtLocation(this, GlockShootSound, GetActorLocation());
						}
						bShootOnce = false;
						CurrentPistolAmmo--;
						CurrentAmmoInPistolClip--;
						if (CurrentAmmoInPistolClip <= 0)
						{
							bReloading = true;
							GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AAICharacter::ResetReloadTimer, ReloadSpeed, false);
							if (GlockReloadSound->IsValidLowLevelFast())
							{
								UGameplayStatics::PlaySoundAtLocation(this, GlockReloadSound, GetActorLocation());
							}
						}
						else
						{
							//bShootOnCooldown = true;
							GetWorld()->GetTimerManager().SetTimer(ShootSpeedTimerHandle, this, &AAICharacter::ResetShootSpeedTimer, 2.0, false);
						}
					}
				}

			}
		}
		if (CurrentWeapon == 1)//Shotgun
		{
			if (bReloading == false)
			{
				if (CurrentFiringMode == 0)
				{
					UWorld* World = GetWorld();
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					if (CurrentProjectileClass->IsValidLowLevelFast() && MuzzleLocation->IsValidLowLevelFast())
					{
						FHitResult Hit;
						UClass* GenerateBPBullet = Cast<UClass>(CurrentProjectileClass);
						World->SpawnActor<AActor>(GenerateBPBullet, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation(), SpawnParams);
						World->SpawnActor<AActor>(GenerateBPBullet, MuzzleLocation1->GetComponentLocation(), MuzzleLocation1->GetComponentRotation(), SpawnParams);
						World->SpawnActor<AActor>(GenerateBPBullet, MuzzleLocation2->GetComponentLocation(), MuzzleLocation2->GetComponentRotation(), SpawnParams);
						World->SpawnActor<AActor>(GenerateBPBullet, MuzzleLocation3->GetComponentLocation(), MuzzleLocation3->GetComponentRotation(), SpawnParams);
						World->SpawnActor<AActor>(GenerateBPBullet, MuzzleLocation4->GetComponentLocation(), MuzzleLocation4->GetComponentRotation(), SpawnParams);

						if (ShotgunShootSound->IsValidLowLevelFast())
						{
							UGameplayStatics::PlaySoundAtLocation(this, ShotgunShootSound, GetActorLocation());
						}
						bShootOnce = false;
						//CurrentShotgunAmmo--;
						CurrentAmmoInShotgunClip--;
						if (CurrentAmmoInShotgunClip <= 0)
						{
							bReloading = true;
							GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AAICharacter::ResetReloadTimer, ReloadSpeed, false);
							if (ShotgunReloadSound->IsValidLowLevelFast())
							{
								UGameplayStatics::PlaySoundAtLocation(this, ShotgunReloadSound, GetActorLocation());
							}
						}
						else
						{
							bShootOnCooldown = true;
							GetWorld()->GetTimerManager().SetTimer(ShootSpeedTimerHandle, this, &AAICharacter::ResetShootSpeedTimer, ShootSpeed, false);
						}
					}
				}
			}

		}

		if (CurrentWeapon == 2)//AK
		{
			if (bReloading == false && bShootOnCooldown == false && CurrentAKAmmo > 0 && CurrentAmmoInAKClip > 0)
			{
				UWorld* World = GetWorld();
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				if (CurrentProjectileClass->IsValidLowLevelFast() && MuzzleLocation->IsValidLowLevelFast())
				{
					FHitResult Hit;
					UClass* GeneratedBPBullet = Cast<UClass>(CurrentProjectileClass);
					World->SpawnActor<AActor>(GeneratedBPBullet, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation(), SpawnParams);
					if (AKShootSound->IsValidLowLevelFast())
					{
						UGameplayStatics::PlaySoundAtLocation(this, AKShootSound, GetActorLocation());
					}
					CurrentAKAmmo--;
					CurrentAmmoInAKClip--;
					if (CurrentAmmoInAKClip <= 0)
					{
						bReloading = true;
						GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AAICharacter::ResetReloadTimer, ReloadSpeed, false);
						if (AKReloadSound->IsValidLowLevelFast()) {
							UGameplayStatics::PlaySoundAtLocation(this, AKReloadSound, GetActorLocation());
						}
					}
					else
					{
						bShootOnCooldown = true;
						GetWorld()->GetTimerManager().SetTimer(ShootSpeedTimerHandle, this, &AAICharacter::ResetShootSpeedTimer, 0.25, false);
					}
				}
			}
		}
		if (CurrentWeapon == 3)//SMG
		{
			if (bReloading == false && bShootOnCooldown == false && CurrentSMGAmmo > 0 && CurrentAmmoInSMGClip > 0)
			{
				UWorld* World = GetWorld();
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				if (CurrentProjectileClass->IsValidLowLevelFast() && MuzzleLocation->IsValidLowLevelFast())
				{
					FHitResult Hit;
					UClass* GeneratedBPBullet = Cast<UClass>(CurrentProjectileClass);
					World->SpawnActor<AActor>(GeneratedBPBullet, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation(), SpawnParams);
					if (UziShootSound->IsValidLowLevelFast())
					{
						UGameplayStatics::PlaySoundAtLocation(this, UziShootSound, GetActorLocation());
					}
					CurrentSMGAmmo--;
					CurrentAmmoInSMGClip--;
					if (CurrentAmmoInSMGClip <= 0)
					{
						bReloading = true;
						GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AAICharacter::ResetReloadTimer, 2.0, false);
						if (UziReloadSound->IsValidLowLevelFast()) {
							UGameplayStatics::PlaySoundAtLocation(this, UziReloadSound, GetActorLocation());
						}
					}
					else
					{
						bShootOnCooldown = true;
						GetWorld()->GetTimerManager().SetTimer(ShootSpeedTimerHandle, this, &AAICharacter::ResetShootSpeedTimer, 0.05, false);
					}
				}
			}
		}

		if (CurrentWeapon == 4) //RPG
		{
			if (bReloading == false && bShootOnCooldown == false && CurrentRPGAmmo > 0 && CurrentAmmoInRPGClip > 0)
			{
				UWorld* World = GetWorld();
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				if (CurrentProjectileClass->IsValidLowLevelFast() && MuzzleLocation->IsValidLowLevelFast())
				{
					FHitResult Hit;
					UClass* GeneratedBPBullet = Cast<UClass>(RPGProjectileClass);
					World->SpawnActor<AActor>(GeneratedBPBullet, MuzzleLocation->GetComponentLocation(), MuzzleLocation->GetComponentRotation(), SpawnParams);
					if (RPGShootSound->IsValidLowLevelFast())
					{
						UGameplayStatics::PlaySoundAtLocation(this, RPGShootSound, GetActorLocation());
					}
					CurrentRPGAmmo--;
					CurrentAmmoInRPGClip--;
					if (CurrentAmmoInRPGClip <= 0)
					{
						bReloading = true;
						GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AAICharacter::ResetReloadTimer, AIRPGReloadSpeed, false);
						if (RPGReloadSound->IsValidLowLevelFast()) {
							UGameplayStatics::PlaySoundAtLocation(this, RPGReloadSound, GetActorLocation());
						}
					}
					else
					{
						bShootOnCooldown = true;
						GetWorld()->GetTimerManager().SetTimer(ShootSpeedTimerHandle, this, &AAICharacter::ResetShootSpeedTimer, AIRPGShootSpeed, false);
					}
				}
			}
		}
	}
}

bool AAICharacter::ReceiveDamage(int amount)
{
	this->currentHealth -= amount;
	if (currentHealth <= 0) {
		bIsDead = true;
		return true;
	}
	return false;
}

void AAICharacter::ResetReloadTimer()
{
	if (!bIsDead) {
		if (CurrentWeapon == 0)
		{
			if (CurrentPistolAmmo >= CurrentPistolClipSize)
			{
				CurrentAmmoInPistolClip = CurrentPistolClipSize;
			}
			else if (CurrentPistolAmmo < CurrentPistolClipSize && CurrentPistolAmmo > 0)
			{
				CurrentAmmoInPistolClip = CurrentPistolAmmo;
			}
			bReloading = false;
			GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
		}
		else if (CurrentWeapon == 1)
		{
			if (CurrentShotgunAmmo >= CurrentShotgunClipSize)
			{
				CurrentAmmoInShotgunClip = CurrentShotgunClipSize;
			}
			else if (CurrentShotgunAmmo < CurrentShotgunClipSize && CurrentShotgunAmmo > 0)
			{
				CurrentAmmoInShotgunClip = CurrentShotgunAmmo;
			}
			bReloading = false;
			GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
		}
		else if (CurrentWeapon == 2)
		{
			if (CurrentAKAmmo >= CurrentAKClipSize)
			{
				CurrentAmmoInAKClip = CurrentAKClipSize;
			}
			else if (CurrentAKAmmo < CurrentAKClipSize && CurrentAKAmmo > 0)
			{
				CurrentAmmoInAKClip = CurrentAKAmmo;
			}
			bReloading = false;
			GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
		}
		else if (CurrentWeapon == 3)
		{
			if (CurrentSMGAmmo >= CurrentSMGClipSize)
			{
				CurrentAmmoInSMGClip = CurrentSMGClipSize;
			}
			else if (CurrentSMGAmmo < CurrentSMGClipSize && CurrentSMGAmmo > 0)
			{
				CurrentAmmoInSMGClip = CurrentSMGAmmo;
			}
			bReloading = false;
			GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
		}
		else if (CurrentWeapon == 4)
		{
			if (CurrentRPGAmmo >= CurrentRPGClipSize)
			{
				CurrentAmmoInRPGClip = CurrentRPGClipSize;
			}
			else if (CurrentRPGAmmo < CurrentRPGClipSize && CurrentRPGAmmo > 0)
			{
				CurrentAmmoInRPGClip = CurrentRPGAmmo;
			}
			bReloading = false;
			GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
		}
	}
}

void AAICharacter::ResetShootSpeedTimer()
{
	bShootOnCooldown = false;
	GetWorldTimerManager().ClearTimer(ShootSpeedTimerHandle);
}

void AAICharacter::Die() {
	if (currentHealth == 0) {
		this->Destroy();
	}
}