// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "GameFramework/Character.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "BlasterCharacter.generated.h"

#define DEFAULT_TURN_THRESHOLD 0.5f
#define DEFAULT_CAMERA_THRESHOLD 200.f
#define DEFAULT_MAX_PLAYER_HEALTH 100.f
#define DEFAULT_DEATH_TIMER_DELAY 2.f

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	/*
	 * Class Defaults
	 */
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	
	/*
	 * Public Overrides
	 */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	virtual void OnRep_ReplicatedMovement() override;

	/*
	 * Public Methods
	 */
	void PlayFireMontage(bool bAiming);
	void PlayDeathMontage();
	void PlayReloadMontage();
	void Elim();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();
	virtual void Destroyed() override;


	UPROPERTY(Replicated)
	bool bDisableGameplay = false;

protected:
	virtual void BeginPlay() override;

	/*
	 * Movement Input Methods
	 */
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	virtual void Jump() override;

	/*
	 * Action Input Methods
	 */
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void ReloadButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void FireButtonPressed();
	void FireButtonReleased();

	/*
	 * Protected Generic Methods
	 */
	void CalculateAO_Pitch();
	void AimOffset(float DeltaTime);
	void SimProxiesTurn();
	void PlayHitReactMontage();

	/*
	 * Player State Methods
	 */
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCauser);
	void UpdateHUDHealth();
	// Poll for any relevant classes and initialize HUD
	void PollInit();
	void RotateInPlace(float DeltaTime);


private:
	/*
	 * Camera Properties
	 */
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	// Overhead Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	/*
	 * Weapon Properties
	 */
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	/*
	 * Combat Properties
	 */
 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCombatComponent* Combat;

	/*
	 * Camera Properties
	 */
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	/*
	* Animation Properties
	*/
	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere)
	float CameraThreshold{DEFAULT_CAMERA_THRESHOLD};

	/*
	* Private Methods
	*/
	float CalculateSpeed();
	void HideCameraIfCharacterClose();

	/*
	* Private Variables
	*/ 

	// Aim Variables
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	// Hit Animation Variables
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float TurnThreshold{DEFAULT_TURN_THRESHOLD};
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	bool bRotateRootBone;

	/*
	 * Player Health
	 */
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = DEFAULT_MAX_PLAYER_HEALTH;

	UPROPERTY(ReplicatedUsing = OnRep_Health, VisibleAnywhere, Category = "Player Stats")
	float Health = DEFAULT_MAX_PLAYER_HEALTH;
	
	UFUNCTION()
	void OnRep_Health();

	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

	bool bWasKilled = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = DEFAULT_DEATH_TIMER_DELAY;
	
	void ElimTimerFinished();

	UPROPERTY()
	class ABlasterPlayerState* BlasterPlayerState;
	
public:
	/*
	 * Getters
	 */
	AWeapon* GetEquippedWeapon();
	FVector GetHitTarget() const;
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() { return bRotateRootBone; }
	FORCEINLINE bool WasKilled() const { return bWasKilled; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	ECombatState GetCombatState() const;
	bool IsWeaponEquipped();
	bool IsAiming();

	/*
	* Setters
	*/
	void SetOverlappingWeapon(AWeapon* Weapon);

};