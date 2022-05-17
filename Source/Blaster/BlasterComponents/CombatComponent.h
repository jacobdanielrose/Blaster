// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "Blaster/Weapon/Weapon.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

#define DEFAULT_ZOOMED_FOV 30.f
#define DEFAULT_ZOOM_INTERP_SPEED 20.f

class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/**
 	* Class Defaults 
 	*/
	UCombatComponent();
	friend class ABlasterCharacter;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
 	* Public Overrides 
 	*/
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/**
 	* Public Methods 
 	*/
	void EquipWeapon( AWeapon* WeaponToEquip );
	void Reload();

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

protected:
	/**
 	* Protected Methods 
 	*/
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);
	void Fire();
	void FireButtonPressed(bool bPressed);
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	void SetHUDCrosshairs(float DeltaTime);

	/**
 	* Replicated Protected Methods 
 	*/
	UFUNCTION(Server, Reliable)
	void ServerReload();
	
	void HandleReload();
	int32 AmountToReload();


	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	
private:
	UPROPERTY()
	class ABlasterCharacter* Character;
	UPROPERTY()
	class ABlasterPlayerController* Controller;
	UPROPERTY()
	class ABlasterHUD* HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	class AWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;
	
	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	/**
	* HUD and crosshairs
	*/

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;
	FVector HitTarget;
	FHUDPackage HUDPackage;

	/*
	 * Aming and FOV
	 */

	// Field of view when not aiming and; set to camera's base FOV in BeginPlay
	float DefaultFOV;
	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV = DEFAULT_ZOOMED_FOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = DEFAULT_ZOOM_INTERP_SPEED;

	void InterpFOV(float DeltaTime);

	/*
	 * Automatic Fire
	 */

	FTimerHandle FireTimer;
	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();
	
	// Carried ammo for the currently-equipped weapon
	UPROPERTY(ReplicatedUsing = OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 StartingARAmmo = 30;

	void InitializeCarriedAmmo();

	UPROPERTY(ReplicatedUsing = OnRep_CombatState)
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	UFUNCTION()
	void OnRep_CombatState();

	void UpdateAmmoValues();

};
