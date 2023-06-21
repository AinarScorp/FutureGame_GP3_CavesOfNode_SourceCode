// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionTrace.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3TEAM11_API UInteractionTrace : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionTrace();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	APlayerCameraManager* cameraManager;

	UPROPERTY(EditAnywhere)
	float interactionLength = 400;

	AActor* focusedActor;
	bool isFocusing;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PerformInteraction();

	void Trace();
};
