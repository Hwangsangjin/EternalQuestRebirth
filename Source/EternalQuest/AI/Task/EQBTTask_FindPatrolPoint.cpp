// Fill out your copyright notice in the Description page of Project Settings.

#include "EQBTTask_FindPatrolPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/EQNonPlayerCharacter.h"
#include "Engine/TargetPoint.h"

UEQBTTask_FindPatrolPoint::UEQBTTask_FindPatrolPoint()
{
}

EBTNodeResult::Type UEQBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	AEQNonPlayerCharacter* Character = Cast<AEQNonPlayerCharacter>(ControlledPawn);
	if (!Character)
	{
		return EBTNodeResult::Failed;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(BlackboardLocation.SelectedKeyName, Character->GetPatrolPoint()->GetActorLocation());
	Character->IncrementPatrolIndex();
	return EBTNodeResult::Succeeded;
}
