// Fill out your copyright notice in the Description page of Project Settings.
//Made by Einar Hallik

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EinarLibrary.generated.h"

UENUM(BlueprintType)
enum class ECustomBranch: uint8
{
	Yes,No
};

/**
 * 
 */
UCLASS(Blueprintable)
class GP3TEAM11_API UEinarLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
#pragma region Raycasts
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Raycasts")
	static FVector GetRaycastNormal(const FHitResult& HitResult);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Raycasts")
	static FVector GetRaycastHitLocation(const FHitResult& HitResult);
#pragma endregion 
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Angle")
	static float GetAngleDegBetweenVectors(FVector VectorA,FVector VectorB);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Vector")
	static FVector GetTargetDirectionProjectedOnActorsPlane(AActor* Target, AActor* ThisActor,float TargetLocationZOffset = -140);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Vector")
	static FVector GetProjectedVectorOnActorsPlane(FVector VectorToProject, AActor* ThisActor);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Vector")
	static FVector GetRightVectorNormalised(FVector Up, FVector Forward);
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Math|Rotation")
	static FRotator GetLerpedRotation(FRotator StartRot, FRotator EndRot,UPARAM(ref) float& Alpha, float AlphaMultiplier, float DeltaTime);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Math|Rotation")
	static FRotator GetRotationOfRotatedActorAroundUpAxis(AActor* ActorRef, float RotationAngleDeg);
	UFUNCTION(BlueprintCallable, Category = "Math|Lerp")
	static float ModifyAlphaLerp(UPARAM(ref) float& Alpha, float ValueToAdd, float ClampMin = 0.f, float ClampMax =1.f);
};
