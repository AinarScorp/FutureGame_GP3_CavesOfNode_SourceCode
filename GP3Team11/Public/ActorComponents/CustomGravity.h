// Fill out your copyright notice in the Description page of Project Settings.
//Made by Einar Hallik

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CustomGravity.generated.h"

UCLASS( ClassGroup=(Custom),Blueprintable, meta=(BlueprintSpawnableComponent) )
class GP3TEAM11_API UCustomGravity : public UActorComponent
{
	GENERATED_BODY()
	
private:
#pragma region Tunning
	UPROPERTY(EditAnywhere, Category = "CustomGravity|Setup")
	TEnumAsByte<ETraceTypeQuery> GroundedTraceChannel = TraceTypeQuery1;
#pragma endregion

#pragma region Tunning
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "CustomGravity|Tunning", meta = (AllowPrivateAccess = "true"))
	float GroundedDistanceCheck = 25.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "CustomGravity|Tunning", meta = (AllowPrivateAccess = "true"))
	float GroundedRadius = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "CustomGravity|Tunning", meta = (AllowPrivateAccess = "true"))
	float GravityForce = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "CustomGravity|Tunning", meta = (AllowPrivateAccess = "true"))
	bool IgnorePlayer = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "CustomGravity|Tunning", meta = (AllowPrivateAccess = "true"))
	float MaxVerticalVelocity = 10.f;
	UPROPERTY(EditAnywhere,Category = "CustomGravity|Tunning", meta = (AllowPrivateAccess = "true"))
	bool ShouldSweep = false;
#pragma endregion
	
#pragma region Debugging
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "CustomGravity|VisibleForDebugging", meta = (AllowPrivateAccess = "true"))
	float VerticalVelocity = 0.f;
	UPROPERTY(EditAnywhere,Category = "CustomGravity|Debugging", meta = (AllowPrivateAccess = "true"))
	bool DrawGroundedCheck = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "CustomGravity|VisibleForDebugging", meta = (AllowPrivateAccess = "true"))
	bool UseGravity = false;
#pragma region endregion

	
	
public:	
	UCustomGravity();

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Meta = (ExpandEnumAsExecs = "Branches"))
	void IsGrounded(ECustomBranch& Branches);
	
	UFUNCTION(BlueprintCallable)
	void SetUseGravity(bool TurnOn);
		
	bool IsGrounded() const;
	void ApplyGravityForce(float DeltaTime);
};
