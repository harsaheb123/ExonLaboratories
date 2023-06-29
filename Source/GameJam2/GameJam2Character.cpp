// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameJam2Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AGameJam2Character::AGameJam2Character()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 1100.f;
	CameraBoom->SetRelativeRotation(FRotator(0.f, 0.f, -80.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a muzzle location
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

	// Create a Flashlight
	//Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	//Flashlight->SetupAttachment(GetCapsuleComponent());
	//Flashlight->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	//Flashlight->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CanDash = true;
	DashDistance = 1500.f;
	DashCoolDown = 1.f;
	DashStop = 0.1f;
}

void AGameJam2Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Player Movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AGameJam2Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGameJam2Character::MoveRight);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AGameJam2Character::Dash);
	//Player shooting control
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AGameJam2Character::ShootPressed);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AGameJam2Character::ShootReleased);
	//Player Weapon Cycle
	PlayerInputComponent->BindAction("WeaponUp", IE_Pressed, this, &AGameJam2Character::WeaponUp);
	PlayerInputComponent->BindAction("WeaponDown", IE_Pressed, this, &AGameJam2Character::WeaponDown);
	PlayerInputComponent->BindAction("SelectPistol", IE_Pressed, this, &AGameJam2Character::SelectPistol);
	PlayerInputComponent->BindAction("SelectAK", IE_Pressed, this, &AGameJam2Character::SelectAK);
	PlayerInputComponent->BindAction("SelectSMG", IE_Pressed, this, &AGameJam2Character::SelectSMG);
	PlayerInputComponent->BindAction("SelectShotgun", IE_Pressed, this, &AGameJam2Character::SelectShotgun);
	PlayerInputComponent->BindAction("SelectRPG", IE_Pressed, this, &AGameJam2Character::SelectRPG);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AGameJam2Character::Reload);
}

void AGameJam2Character::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FHitResult TraceHitResult;
		PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
		FVector CursorFV = TraceHitResult.ImpactNormal;
		FRotator CursorR = CursorFV.Rotation();
		FRotator RotationTemp = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), TraceHitResult.Location);
		float yaw = RotationTemp.Yaw;
		FRotator RotationToLookAt = FRotator(0.f, yaw, 0.f);
		this->SetActorRotation(RotationToLookAt);
	}

	if (bShoot && bDead == false) {
		if (CurrentWeapon == 0)//Pistol
		{
			if (bReloading == false && CurrentPistolAmmo > 0 && CurrentAmmoInPistolClip > 0)
			{
				if (CurrentFiringMode == 0 && bShootOnce)
				{
					UWorld* World = GetWorld();
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					if (CurrentProjectileClass->IsValidLowLevelFast() && MuzzleLocation->IsValidLowLevelFast())
					{
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
							GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AGameJam2Character::ResetReloadTimer, ReloadSpeed, false);
							if (GlockReloadSound->IsValidLowLevelFast())
							{
								UGameplayStatics::PlaySoundAtLocation(this, GlockReloadSound, GetActorLocation());
							}
						}
						else
						{
							bShootOnCooldown = true;
							GetWorld()->GetTimerManager().SetTimer(ShootSpeedTimerHandle, this, &AGameJam2Character::ResetShootSpeedTimer, ShootSpeed, false);
						}
					}
				}
			}
		}
		if (CurrentWeapon == 1)//Shotgun
		{
			if (bReloading == false && CurrentShotgunAmmo > 0 && CurrentAmmoInShotgunClip > 0)
			{
				if (CurrentFiringMode == 0 && bShootOnce)
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
						CurrentShotgunAmmo--;
						CurrentAmmoInShotgunClip--;
						if (CurrentAmmoInShotgunClip <= 0)
						{
							bReloading = true;
							GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AGameJam2Character::ResetReloadTimer, ReloadSpeed, false);
							if (ShotgunReloadSound->IsValidLowLevelFast())
							{
								UGameplayStatics::PlaySoundAtLocation(this, ShotgunReloadSound, GetActorLocation());
							}
						}
						else
						{
							bShootOnCooldown = true;
							GetWorld()->GetTimerManager().SetTimer(ShootSpeedTimerHandle, this, &AGameJam2Character::ResetShootSpeedTimer, ShootSpeed, false);
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
						GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AGameJam2Character::ResetReloadTimer, ReloadSpeed, false);
						if (AKReloadSound->IsValidLowLevelFast()) {
							UGameplayStatics::PlaySoundAtLocation(this, AKReloadSound, GetActorLocation());
						}
					}
					else
					{
						bShootOnCooldown = true;
						GetWorld()->GetTimerManager().SetTimer(ShootSpeedTimerHandle, this, &AGameJam2Character::ResetShootSpeedTimer, ShootSpeed, false);
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
						GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AGameJam2Character::ResetReloadTimer, ReloadSpeed, false);
						if (UziReloadSound->IsValidLowLevelFast()) {
							UGameplayStatics::PlaySoundAtLocation(this, UziReloadSound, GetActorLocation());
						}
					}
					else
					{
						bShootOnCooldown = true;
						GetWorld()->GetTimerManager().SetTimer(ShootSpeedTimerHandle, this, &AGameJam2Character::ResetShootSpeedTimer, SMGShootSpeed, false);
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
						GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AGameJam2Character::ResetReloadTimer, RPGReloadSpeed, false);
						if (RPGReloadSound->IsValidLowLevelFast()) {
							UGameplayStatics::PlaySoundAtLocation(this, RPGReloadSound, GetActorLocation());
						}
					}
					else
					{
						bShootOnCooldown = true;
						GetWorld()->GetTimerManager().SetTimer(ShootSpeedTimerHandle, this, &AGameJam2Character::ResetShootSpeedTimer, RPGShootSpeed, false);
					}
				}
			}
		}
	}
}

void AGameJam2Character::setFiringMode(int mode)
{
	if (mode == 0) { //Single Fire
		CurrentFiringMode = 0;
	}
	else if (mode == 1) { //Automatic
		CurrentFiringMode = 1;
	}
}

void AGameJam2Character::ReceiveDamage(int ammount)
{
	if(CanTakeHits)
	this->CurrentHealth -= ammount;
	if (CurrentHealth <= 0) {
		Die();
	}
}

void AGameJam2Character::Die()
{
	bDead = true;
}

int AGameJam2Character::GetCurrentWeapon()
{
	return this->CurrentWeapon;
}

void AGameJam2Character::ResetShootSpeedTimer()
{
	bShootOnCooldown = false;
	GetWorldTimerManager().ClearTimer(ShootSpeedTimerHandle);
}

void AGameJam2Character::ResetReloadTimer()
{
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

void AGameJam2Character::SetCurrentKeypad(UObject* keypad)
{
	this->CurrentKeypad = keypad;
}

void AGameJam2Character::ClearCurrentKeypad()
{
	this->CurrentKeypad = nullptr;
}


void AGameJam2Character::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && bDead == false)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGameJam2Character::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && bDead == false)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AGameJam2Character::ShootPressed()
{
	this->bShoot = true;
	this->bShootOnce = true;
}

void AGameJam2Character::ShootReleased()
{
	this->bShoot = false;
}

void AGameJam2Character::SelectPistol()
{
	CurrentWeapon = 0;
	CurrentFiringMode = 0;
}

void AGameJam2Character::SelectShotgun()
{
	if (bHasShotgun)
	{
		CurrentWeapon = 1;
		CurrentFiringMode = 0;
	}
}

void AGameJam2Character::SelectAK()
{
	if (bHasAK)
	{
		CurrentWeapon = 2;
		CurrentFiringMode = 1;
	}
}

void AGameJam2Character::SelectSMG()
{
	if (bHasSMG)
	{
		CurrentWeapon = 3;
		CurrentFiringMode = 1;
	}

}
void AGameJam2Character::SelectRPG()
{
	if (bHasRPG)
	{
		CurrentWeapon = 4;
		CurrentFiringMode = 0;
	}
}


void AGameJam2Character::Reload()
{
	bReloading = true;
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AGameJam2Character::ResetReloadTimer, ReloadSpeed, false);
}

void AGameJam2Character::WeaponUp()
{

	UpdateWeaponWheel(true);
	if (WeaponWheel == 0)
	{
		SelectPistol();
	}
	if (WeaponWheel == 1 && bHasShotgun)
	{
		SelectShotgun();
	}

	if (WeaponWheel == 2 && bHasAK)
	{
		SelectAK();
	}

	if (WeaponWheel == 3 && bHasSMG)
	{
		SelectSMG();
	}
	if (WeaponWheel == 4 && bHasRPG)
	{
		SelectRPG();
	}
}

void AGameJam2Character::WeaponDown()
{
	UpdateWeaponWheel(false);
	if (WeaponWheel == 0)
	{
		SelectPistol();
	}
	if (WeaponWheel == 1 && bHasShotgun)
	{
		SelectShotgun();
	}

	if (WeaponWheel == 2 && bHasAK)
	{
		SelectAK();
	}

	if (WeaponWheel == 3 && bHasSMG)
	{
		SelectSMG();
	}
	if (WeaponWheel == 4 && bHasRPG)
	{
		SelectRPG();
	}
}

void AGameJam2Character::UpdateWeaponWheel(bool GoingUp)
{
	if (GoingUp)
	{
		WeaponWheel++;
		if (WeaponWheel > 4)
		{
			WeaponWheel = 0;
		}
	}
	else
	{
		WeaponWheel--;
		if (WeaponWheel < 0)
		{
			WeaponWheel = 4;
		}
	}
}


void AGameJam2Character::Dash()
{
	if (CanDash)
	{
		ACharacter* myCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		FVector Forward = myCharacter->GetActorForwardVector() * DashDistance;
		LaunchCharacter(Forward, true, true);
		CanDash = false;
		CanTakeHits = false;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AGameJam2Character::StopDashing, DashStop, false);
	}
}

void AGameJam2Character::StopDashing()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AGameJam2Character::ResetDash, DashCoolDown, false);
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
}

void AGameJam2Character::ResetDash()
{
	CanDash = true;
	CanTakeHits = true;
}