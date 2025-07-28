// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EQBTTask_UpdateMoveSpeed.generated.h"

UCLASS()
class ETERNALQUEST_API UEQBTTask_UpdateMoveSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UEQBTTask_UpdateMoveSpeed();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, Meta = (AllowPrivateAccess = true))
	float MaxWalkSpeed;
};
