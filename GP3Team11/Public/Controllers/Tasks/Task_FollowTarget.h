// Fill out your copyright notice in the Description page of Project Settings.
//Made by Einar Hallik

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "Task_FollowTarget.generated.h"

/**
 * 
 */
UCLASS()
class GP3TEAM11_API UTask_FollowTarget : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	
	virtual void OnGameplayTaskActivated(UGameplayTask& Task) override;
private:
	UPROPERTY()
	class AEnemyBase* CachedEnemy;
	bool EnemyIsUnderPlayer;
	
	UPROPERTY(EditAnywhere)
	float TargetLocationZOffset = -140;
	UPROPERTY(EditAnywhere)
	float AngleToDetermineIfOnTheCeiling = 95;
	UPROPERTY(EditAnywhere)
	float EnemyZOffset = 0;
	UPROPERTY(EditAnywhere)
	float PlayerZOffset = -80;
public:
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable,BlueprintPure)
	bool ShouldRotateToTarget(AActor* FollowTarget);
private:
#pragma region Getters & Setters
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool GetEnemyIsUnderPlayer() const { return EnemyIsUnderPlayer; }
	UFUNCTION(BlueprintCallable)
	void SetEnemyIsUnderPlayer(const bool NewValue) {EnemyIsUnderPlayer = NewValue;}
	UFUNCTION(BlueprintCallable,BlueprintPure)
	AEnemyBase* GetCachedEnemy() const { return CachedEnemy; }
	UFUNCTION(BlueprintCallable)
	void SetCachedEnemy(AEnemyBase* EnemyBase) {CachedEnemy = EnemyBase;}

#pragma endregion
	UFUNCTION(BlueprintCallable)
	void AssignEnemyPawn(APawn* ControlledPawn);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsOnTheCeiling();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FRotator GetRotationToTarget(AActor* FollowTarget);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool IsCloseToTarget(AActor* Target, float AcceptanceLength);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool ThisEnemyIsAbovePlayer(AActor* Target);
	UFUNCTION(BlueprintCallable,BlueprintPure)
	bool LookingAtTarget(AActor* Target, float AcceptanceAngle = 10);
	FVector GetTargetDirectionProjectedOnPlane(AActor* FollowTarget) const;
};
