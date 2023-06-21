// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class GP3TEAM11_API AEnemyControllerBase : public AAIController
{
	GENERATED_BODY()
private:
#pragma region Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyController|AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EnemyController|AI Behavior", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

#pragma endregion
public:
	AEnemyControllerBase();
	
	virtual void OnPossess(APawn* InPawn) override;
};
