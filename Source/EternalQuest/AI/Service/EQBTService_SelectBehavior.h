// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "EQBTService_SelectBehavior.generated.h"

enum class EAIBehavior : uint8;
class AEQNonPlayerCharacter;

UCLASS()
class ETERNALQUEST_API UEQBTService_SelectBehavior : public UBTService
{
	GENERATED_BODY()

public:
	UEQBTService_SelectBehavior();

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void SetBehaviorKey(UBlackboardComponent* BlackboardComponent, EAIBehavior Behavior) const;
	void UpdateBehavior(UBlackboardComponent* BlackboardComponent) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI, Meta = (AllowPrivateAccess = true))
	TObjectPtr<AEQNonPlayerCharacter> ControlledEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, Meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, Meta = (AllowPrivateAccess = true))
	FBlackboardKeySelector BehaviorKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI, Meta = (AllowPrivateAccess = true))
	float AttackRangeDistance;
};
