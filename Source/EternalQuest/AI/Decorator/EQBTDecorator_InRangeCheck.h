// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "EQBTDecorator_InRangeCheck.generated.h"

UCLASS()
class ETERNALQUEST_API UEQBTDecorator_InRangeCheck : public UBTDecorator
{
	GENERATED_BODY()

public:
	UEQBTDecorator_InRangeCheck();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, Meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector TargetBlackboardKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, Meta = (AllowPrivateAccess = true))
	float MinRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, Meta = (AllowPrivateAccess = true))
	float MaxRange;
};
