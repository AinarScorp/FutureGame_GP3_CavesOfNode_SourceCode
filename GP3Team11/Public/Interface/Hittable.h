// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hittable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHittable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GP3TEAM11_API IHittable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void GetHit(float DamageAmount, AController* EventInstigator, AActor* DamageCauser,const FHitResult& Hit);

};
