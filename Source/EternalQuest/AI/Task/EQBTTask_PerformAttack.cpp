// Fill out your copyright notice in the Description page of Project Settings.

#include "EQBTTask_PerformAttack.h"
#include "AIController.h"
#include "Component/EQStateComponent.h"
#include "Interface/EQCombatCapable.h"

EBTNodeResult::Type UEQBTTask_PerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControlledPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	IEQCombatCapable* CombatInterface = Cast<IEQCombatCapable>(ControlledPawn);
	if (!CombatInterface)
	{
		return EBTNodeResult::Failed;
	}

	TWeakObjectPtr<UBehaviorTreeComponent> WeakOwnerComp = &OwnerComp;

	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindLambda([this, ControlledPawn, WeakOwnerComp](UAnimMontage* Montage, bool bInterrupted)
	{
		if (!WeakOwnerComp.IsValid())
		{
			return;
		}

		if (UEQStateComponent* StateComponent = ControlledPawn->GetComponentByClass<UEQStateComponent>())
		{
			StateComponent->ClearState();
		}

		FinishLatentTask(*WeakOwnerComp.Get(), EBTNodeResult::Succeeded);
	});

	CombatInterface->PerformAttack(AttackTypeTag, OnMontageEnded);
	return EBTNodeResult::InProgress;
}
