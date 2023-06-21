// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CheckpointManager.h"

#include "GP3Team11/TP_FirstPerson/TP_FirstPersonGameMode.h"
#include "Kismet/GameplayStatics.h"


class ATP_FirstPersonCharacter;

void ACheckpointManager::BeginPlay()
{
	Super::BeginPlay();
	Cast<ATP_FirstPersonGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->CheckpointManager = this;
}

void ACheckpointManager::LoadData(ATP_FirstPersonCharacter* Player)
{
	Player->Health->CurrentHealth = CurrentHealth;
	if(isHolstered)
	{
		if(!Player->WeaponSwitching->WeaponsHolstered)
		{
			Player->WeaponSwitching->HolsterWeapon();
		}

		Player->WeaponSwitching->CurrentWeaponIndex = CurrentWeaponIndex;
	}
	else
	{
		while(Player->WeaponSwitching->CurrentWeaponIndex != CurrentWeaponIndex || Player->WeaponSwitching->WeaponsHolstered)
		{
			Player->WeaponSwitching->NextWeapon();
		}
	}

	for(int i=0; i<WeaponDatas.Num(); i++)
	{
		Player->WeaponSwitching->Weapons[i]->CurrentAmmo = WeaponDatas[i];
	}
	Player->SetActorRotation(Rotation,ETeleportType::ResetPhysics);
	Player->SetActorLocation(Position,false,nullptr,ETeleportType::ResetPhysics);
}
