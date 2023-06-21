// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolabelActor.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoolabelActor : public UInterface
{
	GENERATED_BODY()
	
};

/**
 * 
 */
class GP3TEAM11_API IPoolabelActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void SpawnPoolableActor(FTransform Transform);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void SpawnByTheObjectPooling();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AActor* GetActor();
};
