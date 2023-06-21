// Fill out your copyright notice in the Description page of Project Settings.

//Made by Einar Hallik

#include "ActorComponents/PathTracer.h"

#include "CharacterClasses/EnemyBase.h"
#include "Kismet/KismetMathLibrary.h"

UPathTracer::UPathTracer()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UPathTracer::BeginPlay()
{
	Super::BeginPlay();
	CachedEnemy = Cast<AEnemyBase>(GetOwner());
	if (!CachedEnemy)
	{
		UE_LOG(LogTemp,Error,TEXT("Cached enemy has not been assigned"))
		DestroyComponent(false);
	}
	TargetMoveLocation = CachedEnemy->GetActorLocation();
	AlignedRotation = CachedEnemy->GetActorRotation();
}


void UPathTracer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPathTracer::ShootRaycasts_Implementation(bool& SuccessfulHit)
{
}


void UPathTracer::RaycastForward(FHitResult& OutHitResult, bool& SuccessfulHit, float MovementDirScale, FLinearColor TraceColor, float RaycastLength, EDrawDebugTrace::Type DrawDebugTrace)
{
	FVector RayCastEnd = GetMovementDirectionDown(MovementDirScale) * RaycastLength;
	Raycast(RayCastEnd, OutHitResult,SuccessfulHit,TraceColor,DrawDebugTrace);
}

void UPathTracer::RaycastRight(FHitResult& OutHitResult, bool& SuccessfulHit, float MovementDirScale, FLinearColor TraceColor, float RaycastLength, EDrawDebugTrace::Type DrawDebugTrace)
{
	const FVector RotatedVector = CachedEnemy->GetActorRightVector();
	const FVector RayCastEnd = GetMovementDirection(MovementDirScale) + RotatedVector *RaycastLength + GetUpDirection(DownScaleForRaycastForward);

	Raycast(RayCastEnd, OutHitResult,SuccessfulHit,TraceColor,DrawDebugTrace);
}

void UPathTracer::RaycastFromForwardTillAngledDown(FHitResult& OutHitResult, bool& SuccessfulHit, FVector& RotatedVector, float AngleDeg, FVector VectorToRotate, FVector RotateAroundAxis,
	FLinearColor TraceColor, float RaycastLength, EDrawDebugTrace::Type DrawDebugTrace)
{
	FVector RaycastStart;
	CachedEnemy->GetNoseLocation(RaycastStart);
	//TODO Magic numbers
	RaycastStart += GetMovementDirectionDown(1000) * MultiplierOfGetMovementDirectionDown;
	RotatedVector = UKismetMathLibrary::RotateAngleAxis(VectorToRotate,AngleDeg,RotateAroundAxis) * RaycastLength;
	
	const FVector RayCastEnd = RotatedVector + RaycastStart;
	
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(CachedEnemy);
	SuccessfulHit = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(), RaycastStart,RayCastEnd,
		TraceChannel,false,ActorsToIgnore,
		DrawDebugTrace,OutHitResult,true,TraceColor);
}



FVector UPathTracer::GetMovementDirectionDown(float Scale)
{
	//Forward then down
	
	FVector MovementDirDown = GetMovementDirection(Scale) + GetUpDirection(DownScaleForRaycastForward);
	MovementDirDown.Normalize();
	return MovementDirDown;
}



void UPathTracer::ToggleTracing(bool TurnOn)
{
	if (TurnOn)
	{
		CachedEnemy->GetWorldTimerManager().SetTimer(TraceSurfaceTimer,this, &UPathTracer::TraceSurface, TraceSurfaceInRate,true);
	}
	else
	{
		CachedEnemy->GetWorldTimerManager().ClearTimer(TraceSurfaceTimer);
	}
}

void UPathTracer::TraceSurface()
{
	ShootRaycasts(HitsWereSuccessful);
	if (!HitsWereSuccessful)
	{
		return;
	}
	CalculateTargetMoveLocation();
	CalculateAlignedRotation();
}

FVector UPathTracer::GetMovementDirection(float Scale) const
{
	return CachedEnemy->GetActorForwardVector() * Scale;
}

FVector UPathTracer::GetUpDirection(float Scale) const
{
	return CachedEnemy->GetActorUpVector() * Scale;

}


void UPathTracer::CalculateTargetMoveLocation()
{
	//TODO GET BACK TO IT
	
	TargetMoveLocation = HitForwardCloser.Location + HitForwardCloser.Normal * DistanceFromTheGround;
	
}

void UPathTracer::CalculateAlignedRotation()
{
	const FVector Forward = UKismetMathLibrary::GetDirectionUnitVector(HitForwardCloser.Location,HitForwardFurther.Location);
	const FVector Right = UKismetMathLibrary::GetDirectionUnitVector(HitForwardCloser.Location,HitRight.Location);
	const FVector Up = HitForwardCloser.Normal;
	AlignedRotation = UKismetMathLibrary::MakeRotationFromAxes(Forward,Right,Up);
}

void UPathTracer::Raycast(FVector RayCastEnd,FHitResult& OutHitResult, bool& SuccessfulHit, FLinearColor TraceColor, EDrawDebugTrace::Type DrawDebugTrace)
{
	FVector RaycastStart;
	CachedEnemy->GetNoseLocation(RaycastStart);
	
	RayCastEnd+= RaycastStart;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(CachedEnemy);
	
	SuccessfulHit = UKismetSystemLibrary::LineTraceSingle(
	GetWorld(), RaycastStart,RayCastEnd,
	TraceChannel,false,ActorsToIgnore,
	DrawDebugTrace,OutHitResult,true,TraceColor);
}

