// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAnimInstance.h"
#include "Component/EQCombatComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UEQAnimInstance::UEQAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
	CombatType = ECombatType::None;
}

void UEQAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(GetOwningActor());
	if (!Character)
	{
		return;
	}

	CharacterMovementComponent = Character->GetCharacterMovement();

	UEQCombatComponent* CombatComponent = Character->GetComponentByClass<UEQCombatComponent>();
	if (!CombatComponent)
	{
		return;
	}

	CombatComponent->OnChangedCombatDelegate.AddUObject(this, &ThisClass::OnChangedCombat);
}

void UEQAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CharacterMovementComponent)
	{
		return;
	}

	const FVector Velocity = Character->GetVelocity();
	GroundSpeed = Velocity.Size2D();

	const FVector ForwardVector = Character->GetActorForwardVector();
	const FVector RightVector = Character->GetActorRightVector();
	const FVector2D MoveDirection = FVector2D(FVector::DotProduct(ForwardVector, Velocity), FVector::DotProduct(RightVector, Velocity));
	const float DirectionAngle = FMath::Atan2(MoveDirection.Y, MoveDirection.X);
	Direction = FMath::RadiansToDegrees(DirectionAngle);

	bIsMoving = GroundSpeed > MovingThreshold;
	bIsFalling = CharacterMovementComponent->IsFalling();
	bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
}

void UEQAnimInstance::UpdateCombatType(const ECombatType& InCombatType)
{
	CombatType = InCombatType;
}

void UEQAnimInstance::OnChangedCombat(const bool bInCombatEnabled)
{
	bCombatEnabled = bInCombatEnabled;
}
