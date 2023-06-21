// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Stabbable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStabbable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GP3TEAM11_API IStabbable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void ApplyStabbing(class UTP_WeaponComponent* StabWeapon);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool IsLockableStab();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool IsOneTimeStab();
};
