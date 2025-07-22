// Fill out your copyright notice in the Description page of Project Settings.

#include "EQTargetActor_MultiTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Attribute/EQCharacterSkillAttributeSet.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "Physics/EQCollision.h"

AEQTargetActor_MultiTrace::AEQTargetActor_MultiTrace()
{
}

FGameplayAbilityTargetDataHandle AEQTargetActor_MultiTrace::MakeTargetData() const
{
	const ACharacter* Character = CastChecked<ACharacter>(SourceActor);

	const UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!AbilitySystemComponent)
	{
		return FGameplayAbilityTargetDataHandle();
	}

	const UEQCharacterSkillAttributeSet* SkillAttribute = AbilitySystemComponent->GetSet<UEQCharacterSkillAttributeSet>();
	if (!SkillAttribute)
	{
		return FGameplayAbilityTargetDataHandle();
	}

	TArray<FOverlapResult> Overlaps;
	const FVector Origin = Character->GetActorLocation();
	const float SkillRadius = SkillAttribute->GetSkillRange();
	const FCollisionQueryParams Params(SCENE_QUERY_STAT(MultiTrace), false, Character);
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, CCHANNEL_EQACTION, FCollisionShape::MakeSphere(SkillRadius), Params);

	TArray<TWeakObjectPtr<AActor>> HitActors;

	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();
		if (!HitActor || HitActors.Contains(HitActor))
		{
			continue;
		}

		HitActors.Add(HitActor);
	}

	FGameplayAbilityTargetData_ActorArray* TargetData = new FGameplayAbilityTargetData_ActorArray;
	TargetData->SetActors(HitActors);

#if ENABLE_DRAW_DEBUG
	if (bDebugEnabled)
	{
		const FColor DrawColor = HitActors.Num() > 0 ? FColor::Green : FColor::Red;
		DrawDebugSphere(GetWorld(), Origin, SkillRadius, 16, DrawColor, false, 5.0f);
	}
#endif

	return FGameplayAbilityTargetDataHandle(TargetData);
}
