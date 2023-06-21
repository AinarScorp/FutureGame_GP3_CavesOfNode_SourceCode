// Fill out your copyright notice in the Description page of Project Settings.

//Made by Einar Hallik


#include "ActorComponents/ActorPool.h"
#include "Interface/PoolabelActor.h"
#include "Kismet/KismetSystemLibrary.h"

UActorPool::UActorPool()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActorPool::SpawnActorToPool()
{
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorSubclass, FVector().ZeroVector, FRotator().ZeroRotator);
	if (!UKismetSystemLibrary::DoesImplementInterface(SpawnedActor, UPoolabelActor::StaticClass()))
	{
		SpawnedActor->Destroy();
		return;
	}
	IPoolabelActor::Execute_SpawnByTheObjectPooling(SpawnedActor);
	
	SpawnedActor->SetActorTickEnabled(false);
	SpawnedActor->SetActorHiddenInGame(true);
	
	TScriptInterface<IPoolabelActor> PoolabelActor ;
	PoolabelActor.SetObject(SpawnedActor);
	PoolabelActor.SetInterface(Cast<IPoolabelActor>(SpawnedActor));
	Pool.Add(PoolabelActor);
}

void UActorPool::MoveNextPoolableIndex()
{
	NextPoolableIndex++;
	if (NextPoolableIndex>= Pool.Num())
	{
		NextPoolableIndex = 0;
	}
}


// Called when the game starts
void UActorPool::BeginPlay()
{
	Super::BeginPlay();
	if (ActorSubclass == nullptr)
	{
		return;
	}
	if (GetWorld())
	{
		for (int i = 0; i < PoolSize; ++i)
		{
			SpawnActorToPool();
		}
	}
}



TScriptInterface<IPoolabelActor>  UActorPool::GetPooledActor()
{
	if (Pool.IsEmpty() || NextPoolableIndex >= Pool.Num())
	{
		return nullptr;
	}
	TScriptInterface<IPoolabelActor> ActorToReturn = Pool[NextPoolableIndex];
	
	ActorToReturn.GetInterface()->Execute_GetActor(ActorToReturn.GetObject())->SetActorHiddenInGame(false);

	MoveNextPoolableIndex();
	return ActorToReturn;
}
