// Fill out your copyright notice in the Description page of Project Settings.

#include "EQBTService_SelectBehavior.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/EQNonPlayerCharacter.h"

UEQBTService_SelectBehavior::UEQBTService_SelectBehavior()
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UEQBTService_SelectBehavior::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
	{
		return;
	}

	ControlledEnemy = Cast<AEQNonPlayerCharacter>(ControlledPawn);
}

void UEQBTService_SelectBehavior::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UpdateBehavior(OwnerComp.GetBlackboardComponent());
}

void UEQBTService_SelectBehavior::SetBehaviorKey(UBlackboardComponent* BlackboardComponent, EAIBehavior Behavior) const
{
	BlackboardComponent->SetValueAsEnum(BehaviorKey.SelectedKeyName, static_cast<int32>(Behavior));
}

void UEQBTService_SelectBehavior::UpdateBehavior(UBlackboardComponent* BlackboardComponent) const
{
	check(BlackboardComponent);
	check(ControlledEnemy);

	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetKey.SelectedKeyName));
	if (TargetActor)
	{
		const float Distance = TargetActor->GetDistanceTo(ControlledEnemy);

		if (Distance <= AttackRangeDistance)
		{
			SetBehaviorKey(BlackboardComponent, EAIBehavior::MeleeAttack);
		}
		else
		{
			SetBehaviorKey(BlackboardComponent, EAIBehavior::Approach);
		}
	}
	else
	{
		if (ControlledEnemy->GetPatrolPoint())
		{
			SetBehaviorKey(BlackboardComponent, EAIBehavior::Patrol);
		}
		else
		{
			SetBehaviorKey(BlackboardComponent, EAIBehavior::Idle);
		}
	}
}
