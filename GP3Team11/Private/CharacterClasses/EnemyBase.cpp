// Fill out your copyright notice in the Description page of Project Settings.

//Made by Einar Hallik

#include "CharacterClasses/EnemyBase.h"

#include "ActorComponents/HealthComponent.h"
#include "ActorComponents/PathTracer.h"
#include "GP3Team11/TP_FirstPerson/TP_FirstPersonCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Library/EinarLibrary.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	PathTracer = CreateDefaultSubobject<UPathTracer>(TEXT("PathTracer"));
}


void AEnemyBase::GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser,const FHitResult& Hit)
{
	UGameplayStatics::ApplyDamage(this, DamageAmount, EventInstigator, DamageCauser, UDamageType::StaticClass());
}



void AEnemyBase::ApplyStabbing_Implementation(UTP_WeaponComponent* StabWeapon)
{
	UGameplayStatics::ApplyDamage(this, StabWeapon->GetStabDamage(), UGameplayStatics::GetPlayerController(GetWorld(),0), UGameplayStatics::GetPlayerCharacter(GetWorld(),0), UDamageType::StaticClass());
	StabWeapon->ModifyCurrentAmmo(StabWeapon->GetAmmoRefillOnAliveEnemy());
}

bool AEnemyBase::IsLockableStab_Implementation()
{
	return IsLockAbleStab;
}

bool AEnemyBase::IsOneTimeStab_Implementation()
{
	return IsOneHitStab;
}

void AEnemyBase::Explode_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser)
{
	UGameplayStatics::ApplyDamage(this, DamageAmount,EventInstigator,DamageCauser,UDamageType::StaticClass());
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,AActor* DamageCauser)
{
	if (HealthComponent)
	{
		HealthComponent->TakeDamage(DamageAmount);
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemyBase::RotateToTarget()
{
	if (!GetWorld())
	{
		return;
	}
	if (CompletedRotation())
	{
		return;
	}
	FRotator LerpedRot = UEinarLibrary::GetLerpedRotation(CachedStartRot,TargetRotationLerp,AlphaRot,AlphaMultiplier,GetWorld()->GetDeltaSeconds());
	SetActorRotation(LerpedRot);
}

bool AEnemyBase::CompletedRotation()
{
	return AlphaRot>=1.f;
}

void AEnemyBase::SetTargetToRotateTo(FRotator TargetRot, float ChangedAlphaMultiplier)
{
	TargetRotationLerp = TargetRot;
	AlphaMultiplier = ChangedAlphaMultiplier;
	AlphaRot = 0;
	CachedStartRot = GetActorRotation();
}

void AEnemyBase::AlignRotation()
{
	//TODO:Change magic Numbers
	
	const FRotator LerpedRotation= UKismetMathLibrary::RLerp(GetActorRotation(), PathTracer->GetAlignedRotation(),0.05f,true );
	SetActorRotation(LerpedRotation);
}

void AEnemyBase::AddMovementInputToTargetLocation()
{
	FVector NoseLocation;
	GetNoseLocation(NoseLocation);
	const FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(NoseLocation, PathTracer->GetTargetMoveLocation());
	AddMovementInput(Direction);
}

void AEnemyBase::IsMoving(ECustomBranch& Branches)
{
	if (!PathTracer->GetHitsWereSuccessful())
	{
		Branches = ECustomBranch::No;
		return;
	}
	const float DistanceToTarget = UKismetMathLibrary::Vector_Distance(PathTracer->GetTargetMoveLocation(), GetActorLocation());
	Branches = DistanceToTarget> AcceptanceMoveDistanceToTarget? ECustomBranch::Yes:ECustomBranch::No;
}

void AEnemyBase::Move_Implementation()
{
	
}
void AEnemyBase::ToggleMovement_Implementation(bool TurnOn, FRotator TargetRotation, float ChangeAlphaMultiplier)
{
}

void AEnemyBase::PerformAttack_Implementation()
{
	
}

