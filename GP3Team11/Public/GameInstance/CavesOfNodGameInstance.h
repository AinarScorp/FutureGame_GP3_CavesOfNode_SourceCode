#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CavesOfNodGameInstance.generated.h"

UCLASS()
class GP3TEAM11_API UCavesOfNodGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	int RifleAmmo;
};
