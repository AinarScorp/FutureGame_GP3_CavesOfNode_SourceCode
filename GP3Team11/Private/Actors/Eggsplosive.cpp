//Made by Udit Shroff and Einar Hallik

#include "Actors/Eggsplosive.h"
#include "CharacterClasses/EnemyBase.h"
#include "Components/SphereComponent.h"
#include "GP3Team11/TP_FirstPerson/TP_FirstPersonCharacter.h"
#include "Interface/Explodable.h"
#include "Kismet/GameplayStatics.h"

AEggsplosive::AEggsplosive() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");		// set up a notification for when this component hits something blocking
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;
}



void AEggsplosive::AttachToWeapon_Implementation(UTP_WeaponComponent* Weapon)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attaching AttachedEgg!"));
	AttachedWeapon = Weapon;
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,EAttachmentRule::SnapToTarget,EAttachmentRule::KeepWorld, false);
	AttachToComponent(Weapon, AttachmentRules, FName(TEXT("EggPoint")));
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Attached AttachedEgg!"));
	CookEgg();
}

void AEggsplosive::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(bIsCooking)
	{
	 	Eggsplode();
	}
	CollisionComp->OnComponentHit.Clear();
}

void AEggsplosive::CookEgg_Implementation()
{
	bIsCooking = true;
}

void AEggsplosive::Premature_Eggsplode_Implementation()
{
	ThrowEgg(FVector::ZeroVector,0);
	AttachedWeapon->hasEgg = false;
	Eggsplode();
}


void AEggsplosive::Eggsplode_Implementation()
{
	if(bHasEggsploded)
        return;

	bHasEggsploded = true;
	bIsCooking = false;
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);
	
	FVector SphereTraceStart = GetActorLocation();
	FVector SphereTraceEnd = GetActorLocation();
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),SphereTraceStart,SphereTraceEnd, ExplosionRadius, ExplodeTraceChannel,
		false,IgnoreActors,SphereTraceDebug, HitResults,true);

	if (HitResults.IsEmpty())
	{
		return;
	}
	for (FHitResult HitResult : HitResults)
	{
		ExecuteExplodeInterface(HitResult.GetActor());
		ExecuteExplodeInterface(HitResult.GetComponent());
	}
}

bool AEggsplosive::ExecuteExplodeInterface(UObject* PotentialExplodableObject) const
{
	if (UKismetSystemLibrary::DoesImplementInterface(PotentialExplodableObject, UExplodable::StaticClass()))
	{
		IExplodable::Execute_Explode(PotentialExplodableObject,Damage,UGameplayStatics::GetPlayerController(GetWorld(),0),UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
		return true;
	}
	return false;
}
float AEggsplosive::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Eggsplode();
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEggsplosive::GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser,const FHitResult& Hit)
{
	Eggsplode();
}

void AEggsplosive::ApplyStabbing_Implementation(UTP_WeaponComponent* StabWeapon)
{
	IStabbable::ApplyStabbing_Implementation(StabWeapon);
	StabWeapon->AttachEgg(this);
	AttachToWeapon(StabWeapon);
}

bool AEggsplosive::IsLockableStab_Implementation()
{
	return IsLockAbleStab;
}



bool AEggsplosive::IsOneTimeStab_Implementation()
{
	return IsOneHitStab;
}

void AEggsplosive::ThrowEgg_Implementation(FVector Direction, float Speed)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if(!CollisionComp->OnComponentHit.Contains(this, FName("OnHit")))
	{
		CollisionComp->OnComponentHit.AddDynamic(this, &AEggsplosive::OnHit);
	}
	
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
	CollisionComp->AddImpulse(Direction * Speed);
}
