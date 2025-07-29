// Fill out your copyright notice in the Description page of Project Settings.

#include "EQBTDecorator_InRangeCheck.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UEQBTDecorator_InRangeCheck::UEQBTDecorator_InRangeCheck()
{
}

bool UEQBTDecorator_InRangeCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControlledPawn)
	{
		return false;
	}

	const AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetBlackboardKey.SelectedKeyName));
	if (!TargetActor)
	{
		return false;
	}

	const float Distance = ControlledPawn->GetDistanceTo(TargetActor);
	return UKismetMathLibrary::InRange_FloatFloat(Distance, MinRange, MaxRange);
}
