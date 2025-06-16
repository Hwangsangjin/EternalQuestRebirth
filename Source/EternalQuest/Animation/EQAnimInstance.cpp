// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UEQAnimInstance::UEQAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void UEQAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (!Owner)
	{
		return;
	}

	Movement = Owner->GetCharacterMovement();
}

void UEQAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Movement)
	{
		return;
	}

	const FVector Velocity = Owner->GetVelocity();
	GroundSpeed = Velocity.Size2D();

	const FVector ForwardVector = Owner->GetActorForwardVector();
	const FVector RightVector = Owner->GetActorRightVector();
	const FVector2D MoveDirection = FVector2D(FVector::DotProduct(ForwardVector, Velocity), FVector::DotProduct(RightVector, Velocity));
	const float DirectionAngle = FMath::Atan2(MoveDirection.Y, MoveDirection.X);
	Direction = FMath::RadiansToDegrees(DirectionAngle);

	bIsMoving = GroundSpeed > MovingThreshold;
	bIsFalling = Movement->IsFalling();
	bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
}
