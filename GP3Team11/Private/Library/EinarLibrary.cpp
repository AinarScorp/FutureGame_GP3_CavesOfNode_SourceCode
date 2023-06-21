// Fill out your copyright notice in the Description page of Project Settings.
//Made by Einar Hallik

#include "Library/EinarLibrary.h"

#include "Kismet/KismetMathLibrary.h"

FVector UEinarLibrary::GetRaycastNormal(const FHitResult& HitResult)
{
	return HitResult.Normal;
}

FVector UEinarLibrary::GetRaycastHitLocation(const FHitResult& HitResult)
{
	return HitResult.Location;
}

float UEinarLibrary::GetAngleDegBetweenVectors(FVector VectorA, FVector VectorB)
{
	VectorA.Normalize();
	VectorB.Normalize();
	const float DotProduct = UKismetMathLibrary::Dot_VectorVector(VectorA,VectorB);
	return UKismetMathLibrary::DegAcos(DotProduct);
}

FVector UEinarLibrary::GetTargetDirectionProjectedOnActorsPlane(AActor* Target, AActor* ThisActor,float TargetLocationZOffset)
{
	
	if (!Target || !ThisActor)
	{
		UE_LOG(LogTemp,Error,TEXT("Tried to Use GetTargetDirectionProjectedOnActorsPlane with a nullptr"))
		return FVector::Zero();
	}
	FVector TargetLocation = Target->GetActorLocation();
	TargetLocation.Z+= TargetLocationZOffset;
	FVector DirectionToTarget = TargetLocation - ThisActor->GetActorLocation();
	DirectionToTarget.Normalize();
	FVector ProjectedVector = UKismetMathLibrary::ProjectVectorOnToPlane(DirectionToTarget, ThisActor->GetActorUpVector());
	ProjectedVector.Normalize();
	return ProjectedVector;
	//Can replace with this check later
	return GetProjectedVectorOnActorsPlane(DirectionToTarget, ThisActor);
}

FVector UEinarLibrary::GetProjectedVectorOnActorsPlane(FVector VectorToProject, AActor* ThisActor)
{
	if (!ThisActor)
	{
		UE_LOG(LogTemp,Error,TEXT("Tried to Use GetProjectedVectorOnActorsPlane with a nullptr"))
		return FVector::Zero();
	}
	const FVector ProjectedVector = UKismetMathLibrary::ProjectVectorOnToPlane(VectorToProject, ThisActor->GetActorUpVector());
	return ProjectedVector;
}

FVector UEinarLibrary::GetRightVectorNormalised(FVector Up, FVector Forward)
{
	FVector Right = UKismetMathLibrary::Cross_VectorVector(Up,Forward);
	Right.Normalize();
	return Right;
}

FRotator UEinarLibrary::GetLerpedRotation(FRotator StartRot, FRotator EndRot, float& Alpha, float AlphaMultiplier, float DeltaTime)
{
	const FRotator LerpedRotation = UKismetMathLibrary::REase(StartRot,EndRot,Alpha,true,EEasingFunc::Linear);
	ModifyAlphaLerp(Alpha, DeltaTime*AlphaMultiplier);
	return LerpedRotation;
}

FRotator UEinarLibrary::GetRotationOfRotatedActorAroundUpAxis(AActor* ActorRef, float RotationAngleDeg)
{
	const FVector Up = ActorRef->GetActorUpVector();
	const FVector Forward = UKismetMathLibrary::RotateAngleAxis(ActorRef->GetActorForwardVector(), RotationAngleDeg,Up);
	const FVector Right = UKismetMathLibrary::RotateAngleAxis(ActorRef->GetActorRightVector(), RotationAngleDeg,Up);
	
	return UKismetMathLibrary::MakeRotationFromAxes(Forward,Right,Up);
}

float UEinarLibrary::ModifyAlphaLerp(float& Alpha, float ValueToAdd, float ClampMin, float ClampMax)
{
	Alpha+= ValueToAdd;
	Alpha = FMath::Clamp(Alpha,ClampMin,ClampMax);
	return Alpha;
}


