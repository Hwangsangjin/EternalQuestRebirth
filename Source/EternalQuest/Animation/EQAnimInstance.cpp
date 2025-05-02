// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UEQAnimInstance::UEQAnimInstance()
{
	MovingThreshold = 3.0f;
}

void UEQAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (IsValid(Owner))
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UEQAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(Movement))
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();

		bIsMoving = GroundSpeed > MovingThreshold && Movement->GetCurrentAcceleration() != FVector::ZeroVector;

		bIsFalling = Movement->IsFalling();
	}
}
