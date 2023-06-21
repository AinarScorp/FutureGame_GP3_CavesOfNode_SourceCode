// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP3Team11/TP_FirstPerson/TP_FirstPersonCharacter.h"
#include "CheckpointManager.generated.h"

UCLASS()
class GP3TEAM11_API ACheckpointManager : public AActor
{
	GENERATED_BODY()

public:
	FVector Position;
	FRotator Rotation;
	int CurrentHealth;
	int CurrentWeaponIndex;
	bool isHolstered;
	TArray<int> WeaponDatas;

public:
	UFUNCTION(BlueprintCallable)
	void LoadData(ATP_FirstPersonCharacter* Player);
	
protected:
	virtual void BeginPlay() override;
};
