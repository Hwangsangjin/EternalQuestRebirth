// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAbilityTask_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"

UEQAbilityTask_JumpAndWaitForLanding::UEQAbilityTask_JumpAndWaitForLanding()
{
}

UEQAbilityTask_JumpAndWaitForLanding* UEQAbilityTask_JumpAndWaitForLanding::CreateTask(UGameplayAbility* OwningAbility)
{
	UEQAbilityTask_JumpAndWaitForLanding* NewTask = NewAbilityTask<UEQAbilityTask_JumpAndWaitForLanding>(OwningAbility);
	return NewTask;
}

void UEQAbilityTask_JumpAndWaitForLanding::Activate()
{
	Super::Activate();

	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.AddDynamic(this, &ThisClass::OnLandedCallback);
	Character->Jump();

	SetWaitingOnAvatar();
}

void UEQAbilityTask_JumpAndWaitForLanding::OnDestroy(bool bInOwnerFinished)
{
	ACharacter* Character = CastChecked<ACharacter>(GetAvatarActor());
	Character->LandedDelegate.RemoveDynamic(this, &ThisClass::OnLandedCallback);

	Super::OnDestroy(bInOwnerFinished);
}

void UEQAbilityTask_JumpAndWaitForLanding::OnLandedCallback(const FHitResult& Hit)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		JumpAndWaitForLandingDelegate.Broadcast();
	}
}
