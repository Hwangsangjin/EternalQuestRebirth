// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/EQNonPlayerCharacter.h"
#include "Component/EQRotationComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

AEQAIController::AEQAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}

void AEQAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledCharacter = Cast<AEQNonPlayerCharacter>(InPawn);

	RunBehaviorTree(BehaviorTreeAsset);

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ThisClass::UpdateTarget, 0.1f, true);
}

void AEQAIController::OnUnPossess()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);

	Super::OnUnPossess();

	ControlledCharacter = nullptr;
}

void AEQAIController::UpdateTarget() const
{
	TArray<AActor*> Actors;

	AIPerceptionComponent->GetKnownPerceivedActors(nullptr, Actors);

	AEQBaseCharacter* PlayerCharacter = Cast<AEQBaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Actors.Contains(PlayerCharacter))
	{
		if (PlayerCharacter->IsDead())
		{
			SetTargetActor(nullptr);
		}
		else
		{
			SetTargetActor(PlayerCharacter);
		}
	}
	else
	{
		SetTargetActor(nullptr);
	}
}

void AEQAIController::SetTargetActor(AActor* InTargetActor) const
{
	if (!Blackboard)
	{
		return;
	}

	Blackboard->SetValueAsObject(FName("TargetActor"), InTargetActor);

	UEQRotationComponent* RotationComponent = ControlledCharacter->GetComponentByClass<UEQRotationComponent>();
	if (!RotationComponent)
	{
		return;
	}

	RotationComponent->SetTargetActor(InTargetActor);
}
