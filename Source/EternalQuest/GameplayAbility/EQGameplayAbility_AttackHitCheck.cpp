// Fill out your copyright notice in the Description page of Project Settings.

#include "EQGameplayAbility_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilityTask/EQAbilityTask_Trace.h"
#include "Character/EQBaseCharacter.h"
#include "Component/EQStateComponent.h"
#include "GameplayTag/EQGameplayTag.h"

UEQGameplayAbility_AttackHitCheck::UEQGameplayAbility_AttackHitCheck()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UEQGameplayAbility_AttackHitCheck::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	CurrentComboLevel = TriggerEventData ? TriggerEventData->EventMagnitude : 1.0f;

	UEQAbilityTask_Trace* AttackTraceTask = UEQAbilityTask_Trace::CreateTask(this, TargetActorClass);
	AttackTraceTask->TraceResultDelegate.AddDynamic(this, &ThisClass::OnTraceResultCallback);
	AttackTraceTask->ReadyForActivation();
}

void UEQGameplayAbility_AttackHitCheck::OnTraceResultCallback(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		const FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);

		const UAbilitySystemComponent* SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
		UAbilitySystemComponent* TargetSourceAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitResult.GetActor());
		if (!SourceAbilitySystemComponent || !TargetSourceAbilitySystemComponent)
		{
			return;
		}

		AEQBaseCharacter* TargetCharacter = Cast<AEQBaseCharacter>(HitResult.GetActor());
		const AActor* SourceActor = GetAvatarActorFromActorInfo();

		float FinalDamage = 0.0f;

		if (TargetCharacter && TargetCharacter->CanPerformBlock(SourceActor))
		{
			// 블로킹 상태 적용
			TargetCharacter->GetStateComponent()->SetState(EQGameplayTag::Character_State_Blocking);
		}
		else
		{
			// 데미지 적용
			FinalDamage = CurrentComboLevel;

			// 상태 변경
			TargetCharacter->GetStateComponent()->SetState(EQGameplayTag::Character_State_Hit);
		}

		// 히트 리액션
		TargetCharacter->HitReaction(SourceActor);

		// 이동 제한
		TargetCharacter->GetStateComponent()->ToggleMovementInput(false);

		// 데미지 적용 (0일 때는 적용하지 않음)
		if (FinalDamage > 0.0f)
		{
			const FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentComboLevel);
			DamageEffectSpecHandle.Data->SetSetByCallerMagnitude(EQGameplayTag::Character_Data_Damage, FinalDamage);

			ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, DamageEffectSpecHandle, TargetDataHandle);
		}

		// 히트 이펙트 실행
		FGameplayCueParameters CueParameters;
		CueParameters.EffectContext = UAbilitySystemBlueprintLibrary::GetEffectContext(MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentComboLevel));
		CueParameters.EffectContext.AddHitResult(HitResult);
		CueParameters.EffectContext.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());

		TargetSourceAbilitySystemComponent->ExecuteGameplayCue(EQGameplayTag::GameplayCue_Character_AttackHit, CueParameters);
	}
	// else if (UAbilitySystemBlueprintLibrary::TargetDataHasActor(TargetDataHandle, 0))
	// {
	// 	UAbilitySystemComponent* SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	//
	// 	const FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(AttackDamageEffect, CurrentComboLevel);
	// 	if (DamageEffectSpecHandle.IsValid())
	// 	{
	// 		ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, DamageEffectSpecHandle, TargetDataHandle);
	//
	// 		FGameplayEffectContextHandle CueContextHandle = UAbilitySystemBlueprintLibrary::GetEffectContext(DamageEffectSpecHandle);
	// 		CueContextHandle.AddActors(TargetDataHandle.Data[0].Get()->GetActors(), false);
	// 		CueContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	// 		FGameplayCueParameters CueParameters;
	// 		CueParameters.EffectContext = CueContextHandle;
	//
	// 		SourceAbilitySystemComponent->ExecuteGameplayCue(EQGameplayTag::GameplayCue_Character_AttackHit, CueParameters);
	// 	}
	// }

	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
