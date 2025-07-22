// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayAbility/TargetActor/EQTargetActor_Trace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "Attribute/EQCharacterAttributeSet.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Physics/EQCollision.h"

AEQTargetActor_Trace::AEQTargetActor_Trace()
{
}

void AEQTargetActor_Trace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AEQTargetActor_Trace::ConfirmTargetingAndContinue()
{
	if (!SourceActor)
	{
		return;
	}

	const FGameplayAbilityTargetDataHandle TargetDataHandle = MakeTargetData();
	TargetDataReadyDelegate.Broadcast(TargetDataHandle);
}

FGameplayAbilityTargetDataHandle AEQTargetActor_Trace::MakeTargetData() const
{
	const ACharacter* Character = CastChecked<ACharacter>(SourceActor);
	UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	if (!AbilitySystemComponent)
	{
		return FGameplayAbilityTargetDataHandle();
	}

	const UEQCharacterAttributeSet* AttributeSet = AbilitySystemComponent->GetSet<UEQCharacterAttributeSet>();
	if (!AttributeSet)
	{
		return FGameplayAbilityTargetDataHandle();
	}

	FGameplayAbilityTargetDataHandle TargetDataHandle;

	FHitResult HitResult;
	const float AttackRange = AttributeSet->GetAttackRange();
	const float AttackRadius = AttributeSet->GetAttackRadius();

	const FCollisionQueryParams Params(SCENE_QUERY_STAT(SingleTrace), false, Character);
	const FVector ForwardVector = Character->GetActorForwardVector();
	const FVector Start = Character->GetActorLocation() + ForwardVector * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + ForwardVector * AttackRange;

	const bool bHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, CCHANNEL_EQACTION, FCollisionShape::MakeSphere(AttackRadius), Params);

#if ENABLE_DRAW_DEBUG
	if (bDebugEnabled)
	{
		const FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		const float CapsuleHalfHeight = AttackRange * 0.5f;
		const FColor DrawColor = bHit ? FColor::Green : FColor::Red;

		//DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(ForwardVector).ToQuat(), DrawColor, false, 5.0f);
	}
#endif

	if (!bHit)
	{
		return TargetDataHandle;
	}

	TargetDataHandle.Add(new FGameplayAbilityTargetData_SingleTargetHit(HitResult));
	return TargetDataHandle;
}
