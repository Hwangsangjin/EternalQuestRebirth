// Fill out your copyright notice in the Description page of Project Settings.

#include "EQGameplayAbility_Sprint.h"
#include "AbilitySystemComponent.h"
#include "EQGameplayAbility_StaminaRegen.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTag/EQGameplayTag.h"

UEQGameplayAbility_Sprint::UEQGameplayAbility_Sprint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	SprintSpeed = 650.0f;
	NormalSpeed = 500.0f;
}

void UEQGameplayAbility_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}

	if (ActorInfo->AbilitySystemComponent.IsValid())
	{
		const FGameplayAbilitySpec* StaminaRegenSpec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromClass(UEQGameplayAbility_StaminaRegen::StaticClass());
		if (StaminaRegenSpec && StaminaRegenSpec->Ability)
		{
			ActorInfo->AbilitySystemComponent->CancelAbility(StaminaRegenSpec->Ability);
		}
	}

	if (!SprintStaminaDrainEffect)
	{
		return;
	}

	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		return;
	}

	Character->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	if (ActorInfo->AbilitySystemComponent.IsValid())
	{
		StaminaDrainHandle = ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(SprintStaminaDrainEffect->GetDefaultObject<UGameplayEffect>(), 1.0f, ActorInfo->AbilitySystemComponent->MakeEffectContext());
	}
}

void UEQGameplayAbility_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	constexpr bool bReplicateEndAbility = true;
	constexpr bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UEQGameplayAbility_Sprint::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		return false;
	}

	const UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
	if (!MovementComponent)
	{
		return false;
	}

	return MovementComponent->IsMovingOnGround() && !MovementComponent->Velocity.IsNearlyZero();
}

void UEQGameplayAbility_Sprint::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	const ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
	if (!Character)
	{
		return;
	}

	Character->GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	if (ActorInfo->AbilitySystemComponent.IsValid())
	{
		ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTag(EQGameplayTag::Character_State_Sprinting);

		if (StaminaDrainHandle.IsValid())
		{
			ActorInfo->AbilitySystemComponent->RemoveActiveGameplayEffect(StaminaDrainHandle);
			StaminaDrainHandle.Invalidate();
		}

		const FGameplayAbilitySpec* Spec = ActorInfo->AbilitySystemComponent->FindAbilitySpecFromClass(UEQGameplayAbility_StaminaRegen::StaticClass());
		if (Spec)
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec->Handle);
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
