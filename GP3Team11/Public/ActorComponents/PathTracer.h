// Fill out your copyright notice in the Description page of Project Settings.

//Made by Einar Hallik
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PathTracer.generated.h"


UCLASS( ClassGroup=(Custom),Blueprintable, meta=(BlueprintSpawnableComponent) )
class GP3TEAM11_API UPathTracer : public UActorComponent
{
	GENERATED_BODY()
public:

	
private:
#pragma region Setup
	UPROPERTY(EditAnywhere, Category = "PathTracer|Setup")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;
	UPROPERTY(EditAnywhere, Category = "PathTracer|Setup",meta = (ClampMin = 0.f, ClampMax = 3.f))
	float TraceSurfaceInRate =0.1f;
#pragma endregion
#pragma region Tunning

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "PathTracer|Tunning", meta = (ToolTip = "Multiplies UpVector in GetMovementDirectionDown", AllowPrivateAccess = "true"))
	float DownScaleForRaycastForward = -500;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "PathTracer|Tunning", meta = (ToolTip = "Multiplies HitForwardCloser Normal", AllowPrivateAccess = "true"))
	float DistanceFromTheGround = 60;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "PathTracer|Tunning", meta = ( AllowPrivateAccess = "true"))
	float MultiplierOfGetMovementDirectionDown = 50;

#pragma endregion

	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AEnemyBase* CachedEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FVector TargetMoveLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator AlignedRotation;
#pragma region HitResults

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool HitsWereSuccessful;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FHitResult HitForwardCloser;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FHitResult HitForwardFurther;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FHitResult HitRight;
#pragma endregion
	
	FTimerHandle TraceSurfaceTimer;

public:	
	UPathTracer();

protected:
	virtual void BeginPlay() override;

public:
#pragma region Getters & Setters
	FORCEINLINE bool GetHitsWereSuccessful() const { return HitsWereSuccessful; }
	FORCEINLINE FVector GetTargetMoveLocation() const { return TargetMoveLocation; }
	FORCEINLINE FRotator GetAlignedRotation() const { return AlignedRotation; }
#pragma endregion
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void ShootRaycasts(bool& SuccessfulHit);
	UFUNCTION(BlueprintCallable)
	void RaycastForward(FHitResult& OutHitResult, bool& SuccessfulHit, float MovementDirScale = 1000, FLinearColor TraceColor = FLinearColor::Red, float RaycastLength = 200, EDrawDebugTrace::Type DrawDebugTrace = EDrawDebugTrace::Type::None);
	UFUNCTION(BlueprintCallable)
	void RaycastRight(FHitResult& OutHitResult, bool& SuccessfulHit, float MovementDirScale = 1000, FLinearColor TraceColor = FLinearColor::Red, float RaycastLength = 200, EDrawDebugTrace::Type DrawDebugTrace = EDrawDebugTrace::Type::None);
	UFUNCTION(BlueprintCallable)
	void RaycastFromForwardTillAngledDown(FHitResult& OutHitResult, bool& SuccessfulHit,FVector& RotatedVector, float AngleDeg,FVector VectorToRotate, FVector RotateAroundAxis, FLinearColor TraceColor = FLinearColor::Red, float RaycastLength = 200, EDrawDebugTrace::Type DrawDebugTrace = EDrawDebugTrace::Type::None);
	

	UFUNCTION(BlueprintCallable)
	void ToggleTracing(bool TurnOn);
	
	UFUNCTION(BlueprintCallable)
	void TraceSurface();
private:
	UFUNCTION(BlueprintCallable,BlueprintPure)
	FVector GetMovementDirectionDown(float Scale = 1000);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	FVector GetMovementDirection(float Scale = 1) const;
	UFUNCTION(BlueprintCallable,BlueprintPure)
	FVector GetUpDirection(float Scale = 1) const;

	void CalculateTargetMoveLocation();
	void CalculateAlignedRotation();
	void Raycast(FVector RayCastEnd,FHitResult& OutHitResult, bool& SuccessfulHit, FLinearColor TraceColor = FLinearColor::Red, EDrawDebugTrace::Type DrawDebugTrace= EDrawDebugTrace::Type::None);
};
