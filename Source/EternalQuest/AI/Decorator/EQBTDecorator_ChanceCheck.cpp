// Fill out your copyright notice in the Description page of Project Settings.

#include "EQBTDecorator_ChanceCheck.h"

UEQBTDecorator_ChanceCheck::UEQBTDecorator_ChanceCheck()
{
}

bool UEQBTDecorator_ChanceCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return ChanceRate > FMath::RandRange(1.0f, 100.0f);
}
