// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_FirstPersonProjectile.h"

#include "Actors/Eggsplosive.h"
#include "CharacterClasses/EnemyBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ATP_FirstPersonProjectile::ATP_FirstPersonProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ATP_FirstPersonProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	// ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ATP_FirstPersonProjectile::OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))// && OtherComp->IsSimulatingPhysics())
	{
		if(bApplyForce)
		{
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		}

		// if (IHittable* Hittable = Cast<IHittable>(OtherActor))
		// {
		// 	Hittable->Execute_GetHit(OtherActor, Damage,UGameplayStatics::GetPlayerController(GetWorld(),0), UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
		// }

		if (UKismetSystemLibrary::DoesImplementInterface(OtherActor, UHittable::StaticClass()))
		{
			IHittable::Execute_GetHit(OtherActor, Damage,UGameplayStatics::GetPlayerController(GetWorld(),0), UGameplayStatics::GetPlayerCharacter(GetWorld(),0),Hit);
		}
		if (UKismetSystemLibrary::DoesImplementInterface(OtherComp, UHittable::StaticClass()))
		{
			IHittable::Execute_GetHit(OtherComp, Damage,UGameplayStatics::GetPlayerController(GetWorld(),0), UGameplayStatics::GetPlayerCharacter(GetWorld(),0),Hit);
		}
	}
}

void ATP_FirstPersonProjectile::SetupProjectile(FVector Velocity, float GunDamage, bool GunApplyForce)
{
	this->Damage = GunDamage;
	this->bApplyForce = GunApplyForce;
}
