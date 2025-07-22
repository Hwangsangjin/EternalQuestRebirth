// Fill out your copyright notice in the Description page of Project Settings.

#include "EQTargetActor_InteractTrace.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Physics/EQCollision.h"

AEQTargetActor_InteractTrace::AEQTargetActor_InteractTrace()
{
	ObjectType = UEngineTypes::ConvertToObjectType(CCHANNEL_EQPICKUP);
	TraceRadius = 100.0f;
}

void AEQTargetActor_InteractTrace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AEQTargetActor_InteractTrace::ConfirmTargetingAndContinue()
{
	if (!SourceActor)
	{
		return;
	}

	const FGameplayAbilityTargetDataHandle TargetDataHandle = MakeTargetData();
	TargetDataReadyDelegate.Broadcast(TargetDataHandle);
}

FGameplayAbilityTargetDataHandle AEQTargetActor_InteractTrace::MakeTargetData() const
{
	const ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	FGameplayAbilityTargetDataHandle TargetDataHandle;

	const FVector Start = Character->GetActorLocation();
	const FVector End = Start;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(ObjectType);

	const TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;

	const bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(this, Start, End, TraceRadius, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, HitResult, true);
	if (!bHit || !HitResult.GetActor())
	{
		return TargetDataHandle;
	}

	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
	TargetDataHandle.Add(TargetData);

	return TargetDataHandle;
}
