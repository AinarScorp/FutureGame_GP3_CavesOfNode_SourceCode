// Fill out your copyright notice in the Description page of Project Settings.

//Made by Einar Hallik
#include "Controllers/Tasks/Task_FollowTarget.h"

#include "CharacterClasses/EnemyBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Library/EinarLibrary.h"

void UTask_FollowTarget::OnGameplayTaskActivated(UGameplayTask& Task)
{
	Super::OnGameplayTaskActivated(Task);
}

void UTask_FollowTarget::AssignEnemyPawn(APawn* ControlledPawn)
{
	if (GetCachedEnemy())
	{
		return;
	}
	SetCachedEnemy(Cast<AEnemyBase>(ControlledPawn));
	if (!GetCachedEnemy())
	{
		FinishExecute(false);
	}
}

bool UTask_FollowTarget::IsOnTheCeiling()
{
	const FVector ThisActorUp = CachedEnemy->GetActorUpVector();
	const FVector WorldUp = UKismetMathLibrary::Vector_Up();
	const float AngleDifferenceBetweenUpVectors = UEinarLibrary::GetAngleDegBetweenVectors(ThisActorUp,WorldUp);
	return AngleDifferenceBetweenUpVectors >=AngleToDetermineIfOnTheCeiling;
}

FRotator UTask_FollowTarget::GetRotationToTarget(AActor* FollowTarget)
{
	const FVector TargetDirectionOnPlane = GetTargetDirectionProjectedOnPlane(FollowTarget);
	const FVector Up = CachedEnemy->GetActorUpVector();
	const FVector Forward = EnemyIsUnderPlayer?
		                        UKismetMathLibrary::RotateAngleAxis(TargetDirectionOnPlane, 180, Up)
		                        :TargetDirectionOnPlane;
	const FVector Right = UEinarLibrary::GetRightVectorNormalised(Up,Forward);
	const FRotator RotationToTarget = UKismetMathLibrary::MakeRotationFromAxes(Forward,Right,Up);
	return RotationToTarget;
}

bool UTask_FollowTarget::IsCloseToTarget(AActor* Target, float AcceptanceLength)
{
	const FVector VectorToTarget = Target->GetActorLocation() - CachedEnemy->GetActorLocation();
	const FVector ToTargetProjectedOnPlane = UEinarLibrary::GetProjectedVectorOnActorsPlane(VectorToTarget, CachedEnemy);
	const float DistanceToPlayer = ToTargetProjectedOnPlane.Length();
	return DistanceToPlayer<AcceptanceLength;
}


bool UTask_FollowTarget::ThisEnemyIsAbovePlayer(AActor* Target)
{
	const float PlayerZValue = Target->GetActorLocation().Z + PlayerZOffset;
	const float ThisEnemyZValue = CachedEnemy->GetActorLocation().Z + EnemyZOffset;
	
	return ThisEnemyZValue>PlayerZValue;
}

bool UTask_FollowTarget::LookingAtTarget(AActor* Target, float AcceptanceAngle)
{
	const float AngleBetweenThisAndPlayer = UEinarLibrary::GetAngleDegBetweenVectors(GetTargetDirectionProjectedOnPlane(Target), CachedEnemy->GetActorForwardVector());
	return AcceptanceAngle>AngleBetweenThisAndPlayer;
}

FVector UTask_FollowTarget::GetTargetDirectionProjectedOnPlane(AActor* FollowTarget) const
{
	return UEinarLibrary::GetTargetDirectionProjectedOnActorsPlane(FollowTarget, CachedEnemy, TargetLocationZOffset);
}


