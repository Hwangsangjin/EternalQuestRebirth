// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayCue/EQGameplayCueNotify_AttackHit.h"
#include "NiagaraFunctionLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"

UEQGameplayCueNotify_AttackHit::UEQGameplayCueNotify_AttackHit()
{
}

bool UEQGameplayCueNotify_AttackHit::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const
{
	check(NiagaraSystem);
	check(CameraShakeClass);

	const APlayerController* PlayerController = nullptr;
	if (const UWorld* World = GetWorld())
	{
		PlayerController = World->GetFirstPlayerController();
	}

	const FHitResult* HitResult = Parameters.EffectContext.GetHitResult();
	if (HitResult)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(MyTarget->GetWorld(), NiagaraSystem, HitResult->ImpactPoint, MyTarget->GetActorRotation());
	}
	else
	{
		for (const TWeakObjectPtr<AActor>& TargetActor : Parameters.EffectContext.Get()->GetActors())
		{
			if (TargetActor.Get())
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(MyTarget->GetWorld(), NiagaraSystem, TargetActor.Get()->GetActorLocation(), MyTarget->GetActorRotation());
			}
		}
	}

	if (PlayerController && PlayerController->IsLocalController())
	{
		PlayerController->PlayerCameraManager->StartCameraShake(CameraShakeClass);
	}

	return true;
}
