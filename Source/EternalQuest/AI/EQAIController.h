// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "EQAIController.generated.h"

class AEQNonPlayerCharacter;

UCLASS()
class ETERNALQUEST_API AEQAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEQAIController();


protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void UpdateTarget() const;
	void SetTargetActor(AActor* InTargetActor) const;

private:
	UPROPERTY(EditAnywhere, Category = AI, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, Category = AI, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, Category = AI, Meta = (AllowPrivateAccess = true))
	TObjectPtr<AEQNonPlayerCharacter> ControlledCharacter;

	FTimerHandle TimerHandle;
};
