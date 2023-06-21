// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Checkpoint.h"

#include "Actors/CheckpointManager.h"
#include "Components/ShapeComponent.h"
#include "GP3Team11/TP_FirstPerson/TP_FirstPersonCharacter.h"
#include "GP3Team11/TP_FirstPerson/TP_FirstPersonGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
    
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnComponentBeginOverlap);
}

void ACheckpoint::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ATP_FirstPersonCharacter* Player = Cast<ATP_FirstPersonCharacter>(OtherActor))
	{
		SaveData(Player);
	}
}

void ACheckpoint::SaveData(ATP_FirstPersonCharacter* Player)
{
	ACheckpointManager* CheckpointManager = Cast<ATP_FirstPersonGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->CheckpointManager;
	CheckpointManager->Position = GetActorLocation();
	CheckpointManager->Rotation = GetActorRotation();
	CheckpointManager->CurrentHealth = UKismetMathLibrary::Max(50,Player->Health->CurrentHealth);
	CheckpointManager->CurrentWeaponIndex = Player->WeaponSwitching->CurrentWeaponIndex;
	CheckpointManager->isHolstered = Player->WeaponSwitching->WeaponsHolstered;
	CheckpointManager->WeaponDatas.Empty();
	for(int i=0; i<Player->WeaponSwitching->Weapons.Num(); i++)
	{
		CheckpointManager->WeaponDatas.Add(Player->WeaponSwitching->Weapons[i]->CurrentAmmo);
	}
}



