// Fill out your copyright notice in the Description page of Project Settings.

//Made by Einar Hallik

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorPool.generated.h"

class IPoolabelActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3TEAM11_API UActorPool : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditDefaultsOnly, Category = "ActorPool|Setup", meta = (MustImplement ="PoolabelActor"))
	TSubclassOf<AActor> ActorSubclass;
	UPROPERTY(EditDefaultsOnly, Category = "ActorPool|Settings",meta =(ClampMin = 1, ClampMax = 100))
	int PoolSize = 20;
	UPROPERTY()
	TArray<TScriptInterface<IPoolabelActor>> Pool;
	
	int NextPoolableIndex = 0;
	
public:	
	UActorPool();

	UFUNCTION(BlueprintCallable)
	TScriptInterface<IPoolabelActor> GetPooledActor();
		
private:
	void SpawnActorToPool();
	void MoveNextPoolableIndex();
protected:
	virtual void BeginPlay() override;
};
