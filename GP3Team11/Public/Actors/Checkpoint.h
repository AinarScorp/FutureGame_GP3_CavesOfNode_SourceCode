// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "GP3Team11/TP_FirstPerson/TP_FirstPersonCharacter.h"
#include "Checkpoint.generated.h"

UCLASS()
class GP3TEAM11_API ACheckpoint : public ATriggerBox
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void SaveData(ATP_FirstPersonCharacter* Player);
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};