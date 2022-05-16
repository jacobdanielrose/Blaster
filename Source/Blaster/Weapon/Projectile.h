// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

#define DEFAULT_WEAPON_DAMAGE 20.f

/**
 * 
 */
UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	/*
	 * Class Defaults
	 */
	AProjectile();
	virtual void Tick(float DeltaTime) override;
	
	/*
	 * Public Overrides
	 */
	virtual void Destroyed() override;

protected:
	/*
	 * Protected Overrides
	 */
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float Damage = DEFAULT_WEAPON_DAMAGE;
	
private:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	/*
	 * Hit FX Variables
	 */
	UPROPERTY(EditAnywhere)
	class UParticleSystem* Tracer;

	class UParticleSystemComponent* TracerComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticles;

	// soundbase vs soundcue?
	UPROPERTY(EditAnywhere)
	class USoundBase* ImpactSound;

};
