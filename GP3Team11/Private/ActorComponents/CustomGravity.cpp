// Fill out your copyright notice in the Description page of Project Settings.
//Made by Einar Hallik


#include "ActorComponents/CustomGravity.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Library/EinarLibrary.h"

UCustomGravity::UCustomGravity()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomGravity::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (UseGravity)
	{
		ApplyGravityForce(DeltaTime);
	}
}

void UCustomGravity::IsGrounded(ECustomBranch& Branches)
{
	Branches = IsGrounded()? ECustomBranch::Yes : ECustomBranch::No;
}

bool UCustomGravity::IsGrounded() const
{
	if (!GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("Owner has not been found for a ground check inside Custom Gravity c++ class"))
		return false;
	}
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	if (IgnorePlayer)
	{
		ActorsToIgnore.Add(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	}
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	const FVector EndTraceLocation = OwnerLocation - GetOwner()->GetActorUpVector() * GroundedDistanceCheck;
	
	FHitResult OutHitResult;
	
	return UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(), OwnerLocation, EndTraceLocation, GroundedRadius,
		GroundedTraceChannel, false, ActorsToIgnore,
		DrawGroundedCheck ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None,
		OutHitResult, true);
}

void UCustomGravity::ApplyGravityForce(float DeltaTime)
{
	if (GravityForce <= 0.0)
	{
		SetUseGravity(false);
		return;
	}
	if (!GetOwner())
	{
		return;
	}
	if (IsGrounded())
	{
		VerticalVelocity = 0.0;
		return;
	}
	const float AddedVerticalVelocity = VerticalVelocity + DeltaTime * GravityForce;
	VerticalVelocity = FMath::Clamp(AddedVerticalVelocity, 0, MaxVerticalVelocity);
	
	FVector ActorNewLocation = GetOwner()->GetActorLocation();
	ActorNewLocation -= GetOwner()->GetActorUpVector()*VerticalVelocity;

	GetOwner()->SetActorLocation(ActorNewLocation, ShouldSweep);
}

void UCustomGravity::SetUseGravity(bool TurnOn)
{
	UseGravity = TurnOn;
	if (!UseGravity)
	{
		VerticalVelocity = 0.0;
	}
}
