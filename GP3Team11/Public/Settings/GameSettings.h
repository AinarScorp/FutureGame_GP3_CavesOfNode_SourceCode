// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "GameSettings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GP3TEAM11_API UGameSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:
	UPROPERTY(Config,BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float Sensitivity;

	UGameSettings();

	UFUNCTION(BlueprintCallable)
	static UGameSettings* GetGameSettings();
};
