// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/EnemyControllerBase.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CharacterClasses/EnemyBase.h"

AEnemyControllerBase::AEnemyControllerBase()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

}

void AEnemyControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn == nullptr) return;

	const AEnemyBase* Enemy = Cast<AEnemyBase>(InPawn);
	if (Enemy && Enemy->GetBehaviorTree())
	{
		BlackboardComponent->InitializeBlackboard(*(Enemy->GetBehaviorTree()->BlackboardAsset));
	}
}
