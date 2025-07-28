// Fill out your copyright notice in the Description page of Project Settings.

#include "EQBTTask_UpdateMoveSpeed.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UEQBTTask_UpdateMoveSpeed::UEQBTTask_UpdateMoveSpeed()
{
	MaxWalkSpeed = 200.0f;
}

EBTNodeResult::Type UEQBTTask_UpdateMoveSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const APawn* ControlledPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (!ControlledPawn)
	{
		return EBTNodeResult::Failed;
	}

	UCharacterMovementComponent* CharacterMovementComponent = ControlledPawn->GetComponentByClass<UCharacterMovementComponent>();
	if (!CharacterMovementComponent)
	{
		return EBTNodeResult::Failed;
	}

	CharacterMovementComponent->MaxWalkSpeed = MaxWalkSpeed;
	return EBTNodeResult::Succeeded;
}
