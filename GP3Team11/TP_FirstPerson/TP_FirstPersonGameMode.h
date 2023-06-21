// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CheckpointManager.h"
#include "GameFramework/GameModeBase.h"
#include "TP_FirstPersonGameMode.generated.h"

class UWeaponManager;
UCLASS(minimalapi)
class ATP_FirstPersonGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	ACheckpointManager* CheckpointManager;
	
public:
	ATP_FirstPersonGameMode();
};



