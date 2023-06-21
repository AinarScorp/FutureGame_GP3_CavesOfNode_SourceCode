// Fill out your copyright notice in the Description page of Project Settings.

//Made by Einar Hallik
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interface/Explodable.h"
#include "Interface/Hittable.h"
#include "Interface/Stabbable.h"
#include "EnemyBase.generated.h"

class UPathTracer;
class UHealthComponent;
class UBehaviorTree;
UCLASS()
class GP3TEAM11_API AEnemyBase : public APawn, public IHittable,public IStabbable,public IExplodable
{
	GENERATED_BODY()

public:
#pragma region Components
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "EnemyBase|Components",meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "EnemyBase|Components",meta = (AllowPrivateAccess = "true"))
	UPathTracer* PathTracer;
#pragma endregion
	
private:
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "EnemyBase|BehaviourTree|Setup", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BehaviorTree;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName PlayerTagName = "Player";

#pragma region Tunning

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="EnemyBase|BehaviourTree|Tunning", meta = (AllowPrivateAccess = "true"))
	bool CanDropOnTarget = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="EnemyBase|BehaviourTree|Tunning", meta = (AllowPrivateAccess = "true"))
	float AcceptanceLengthForDrop = 350.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="EnemyBase|BehaviourTree|Tunning", meta = (AllowPrivateAccess = "true"))
	bool ShouldWander = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="EnemyBase|BehaviourTree|Tunning", meta = (AllowPrivateAccess = "true"))
	float WanderDurationMax = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="EnemyBase|BehaviourTree|Tunning", meta = (AllowPrivateAccess = "true"))
	float WanderDurationMin =5.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="EnemyBase|WallClimbing|Tunning", meta = (AllowPrivateAccess = "true"))
	float AlphaMultiplier = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="EnemyBase|WallClimbing|Tunning", meta = (AllowPrivateAccess = "true"))
	float AcceptanceMoveDistanceToTarget = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="EnemyBase|Stab|Tunning", meta = (AllowPrivateAccess = "true")) // add , EditCondition = "IsStabbableWeapon",EditConditionHides
	bool IsLockAbleStab = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="EnemyBase|Stab|Tunning", meta = (AllowPrivateAccess = "true"))
	bool IsOneHitStab = false;

#pragma endregion
#pragma region VisibleForDubbing

	UPROPERTY(VisibleAnywhere, Category ="EnemyBase|WallClimbing|VisibleForDebugging")
	float AlphaRot = 0;
#pragma endregion

	FRotator TargetRotationLerp;
	FRotator CachedStartRot;
public:
	AEnemyBase();
	

public:
#pragma region Getters & Setters
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	
	UFUNCTION(BlueprintCallable,BlueprintPure,BlueprintImplementableEvent,Category = "EnemyBase|WallClimbing")
	void GetNoseLocation(FVector& NoseLocation);

#pragma endregion

#pragma region Interfaces
	virtual void GetHit_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser,const FHitResult& Hit) override;
	virtual void ApplyStabbing_Implementation(UTP_WeaponComponent* StabWeapon) override;
	virtual bool IsLockableStab_Implementation() override;
	virtual bool IsOneTimeStab_Implementation() override;
	virtual void Explode_Implementation(float DamageAmount, AController* EventInstigator, AActor* DamageCauser) override;
#pragma endregion 

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PerformAttack();

#pragma region WallMovememnt
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void Move();
	UFUNCTION(BlueprintCallable)
	void RotateToTarget();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CompletedRotation();
	UFUNCTION(BlueprintCallable)
	void SetTargetToRotateTo(FRotator TargetRot, float ChangedAlphaMultiplier = 2.5);
	

	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category = "EnemyBase|WallClimbing")
	void ToggleMovement(bool TurnOn, FRotator TargetRotation, float ChangeAlphaMultiplier = 2.5f);
	
	UFUNCTION(BlueprintCallable, Category = "EnemyBase|WallClimbing", Meta = (ExpandEnumAsExecs = "Branches"))
	void IsMoving(ECustomBranch& Branches);
	
	UFUNCTION(BlueprintCallable, Category = "EnemyBase|WallClimbing")
	void AlignRotation();
	UFUNCTION(BlueprintCallable, Category = "EnemyBase|WallClimbing")
	void AddMovementInputToTargetLocation();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent,Category = "EnemyBase|Behaviour")
	void SetEmergencyFall(bool Value);


};
