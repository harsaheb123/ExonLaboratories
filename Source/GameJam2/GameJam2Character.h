// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameJam2Character.generated.h"

UCLASS(Blueprintable)
class AGameJam2Character : public ACharacter
{
	GENERATED_BODY()

public:
	AGameJam2Character();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//Setup A basic Bullet Projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		UClass* BulletProjectileClass;

	//Set up a current projectile reference so that the projectile can be changed for each weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		UClass* CurrentProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		UClass* RPGProjectileClass;


	//Set the firing mode (0 - Single fire, 1 - Automatic)
	void setFiringMode(int mode);

	//Placeholder Variable for ammo, more will need to be added for different weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
		int CurrentHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
		int MaxHealth = 100;

	UFUNCTION(BlueprintCallable)
		void ReceiveDamage(int ammount);

	UFUNCTION(BlueprintCallable)
		void Die();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerStats, meta = (AllowPrivateAccess = "true"))
		bool bDead = false;

	//Placeholder Variable for ammo, more will need to be added for different weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentPistolAmmo = 28;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentAKAmmo = 120;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentSMGAmmo = 80;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentShotgunAmmo = 24;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentRPGAmmo = 5;

	//Placeholder Variable for ammo, more will need to be added for different weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentPistolMaxAmmo = 28;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentAKMaxAmmo = 120;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentSMGMaxAmmo = 80;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentShotgunMaxAmmo = 24;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentRPGMaxAmmo = 5;
	//Placeholder Variable for Clip size, more will need to be added for different weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentPistolClipSize = 7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentAKClipSize = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentSMGClipSize = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentShotgunClipSize = 6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentRPGClipSize = 1;

	//Placeholder Variable for Clip size, more will need to be added for different weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentAmmoInPistolClip = 7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentAmmoInAKClip = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentAmmoInSMGClip = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentAmmoInShotgunClip = 6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		int CurrentAmmoInRPGClip = 1;

	//Current firing mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
	int CurrentWeapon = 0;
	int CurrentFiringMode = 0;

	UFUNCTION(BlueprintCallable)
		int GetCurrentWeapon();


	//Used in timer to determine time between bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		float ShootSpeed = 0.1;
	float SMGShootSpeed = 0.05;
	float RPGShootSpeed = 1.0;
	bool bShootOnCooldown;
	FTimerHandle ShootSpeedTimerHandle;
	void ResetShootSpeedTimer();

	//Used in timer to determine time between bullets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		float ReloadSpeed = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		float RPGReloadSpeed = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
	bool bReloading;
	FTimerHandle ReloadTimerHandle;
	void ResetReloadTimer();

	// helper variable for singe fire shooting
	bool bShootOnce = true;
	int WeaponWheel = 0;

	//setup sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		class USoundBase* AKShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		class USoundBase* AKReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		class USoundBase* GlockShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		class USoundBase* GlockReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		class USoundBase* UziShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		class USoundBase* UziReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		class USoundBase* ShotgunShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		class USoundBase* ShotgunReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		class USoundBase* RPGShootSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sounds, meta = (AllowPrivateAccess = "true"))
		class USoundBase* RPGReloadSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		bool bHasShotgun = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		bool bHasAK = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		bool bHasSMG = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		bool bHasRPG = false;

	UFUNCTION(BlueprintCallable)
		void SetCurrentKeypad(UObject* keypad);

	UFUNCTION(BlueprintCallable)
		void ClearCurrentKeypad();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UObject* CurrentKeypad = nullptr;
private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Shoot point for bullet to be spawned at */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		USceneComponent* MuzzleLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		USceneComponent* MuzzleLocation1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		USceneComponent* MuzzleLocation2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		USceneComponent* MuzzleLocation3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		USceneComponent* MuzzleLocation4;

	bool CanTakeHits = true;


	/** Flashlight */
	// COMMENTED OUT BECAUSE IT WOULDNT SETUP PARENT COMPONENT PROPERLY, NOW ADDED WITHIN BLUEPRINT
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class USpotLightComponent* Flashlight;

	//Set Up Players Input Component
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** Called whenever the shoot button is clicked down */
	void ShootPressed();

	/** Called whenever the shoot button is released */
	void ShootReleased();

	/** Called whenever 1 is pressed to select pistol */
	void SelectPistol();

	/* Called whenever 2 is pressed to select shotgun*/
	void SelectShotgun();

	/** Called whenever 2 is pressed to select AK */
	void SelectAK();

	/** Called whenever 3 is pressed to select SMG */
	void SelectSMG();

	/*Called whenever 5 is pressed to select RPG */
	void SelectRPG();

	/** Called whenever 3 is pressed to reload current weapon */
	void Reload();

	void WeaponUp();

	void WeaponDown();

	void UpdateWeaponWheel(bool GoingUp);
	//Shoot?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Shooting, meta = (AllowPrivateAccess = "true"))
		bool bShoot;

	UFUNCTION()
		void Dash();

	UPROPERTY(EditAnywhere)
		float DashDistance;

	UPROPERTY(EditAnywhere)
		float DashCoolDown;

	UPROPERTY()
		bool CanDash;

	UPROPERTY(EditAnywhere)
		float DashStop;

	UPROPERTY()
		FTimerHandle UnusedHandle;

	UFUNCTION()
		void StopDashing();

	UFUNCTION()
		void ResetDash();

};

