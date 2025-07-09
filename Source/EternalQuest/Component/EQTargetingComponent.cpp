// Fill out your copyright notice in the Description page of Project Settings.

#include "EQTargetingComponent.h"
#include "EQRotationComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/EQLockOnTargetable.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Physics/EQCollision.h"

UEQTargetingComponent::UEQTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TargetingRadius = 2000.0f;
	TargetInterpSpeed = 20.0f;
}

void UEQTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());
	if (!Owner)
	{
		return;
	}

	CameraComponent = Owner->GetComponentByClass<UCameraComponent>();
}

void UEQTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsLockOn)
	{
		return;
	}

	if (!Owner)
	{
		return;
	}

	if (!LockedTargetActor)
	{
		return;
	}

	const float Distance = FVector::Distance(Owner->GetActorLocation(), LockedTargetActor->GetActorLocation());

	IEQLockOnTargetable* LockOnTarget = Cast<IEQLockOnTargetable>(LockedTargetActor);
	if (!LockOnTarget)
	{
		return;
	}

	if (!LockOnTarget->CanBeTargeted() || Distance > TargetingRadius)
	{
		StopLockOn();
	}
	else
	{
		LookAtLockedTarget();
	}
}

void UEQTargetingComponent::ToggleLockOn()
{
	if (bIsLockOn)
	{
		StopLockOn();
	}
	else
	{
		StartLockOn();
	}
}

void UEQTargetingComponent::SwitchingLockedOnActor(ESwitchingDirection InDirection)
{
	if (!LockedTargetActor)
	{
		return;
	}

	IEQLockOnTargetable* LockOnTarget = Cast<IEQLockOnTargetable>(LockedTargetActor);
	if (!LockOnTarget)
	{
		return;
	}

	// 기존 타겟 해제
	LockOnTarget->OnTargeted(false);

	// 신규 타겟 후보 찾기
	TArray<AActor*> TargetActors;
	FindTargets(TargetActors);

	// 방향에 따른 가장 가까운 타겟 찾기
	AActor* TargetActor = FindClosestTarget(TargetActors, InDirection);
	if (TargetActor)
	{
		// 인터페이스 확인 후 락온 시작 신호 보내기
		if (IEQLockOnTargetable* NewLockOnTarget = Cast<IEQLockOnTargetable>(TargetActor))
		{
			NewLockOnTarget->OnTargeted(true);

			// 락온 대상 갱신
			LockedTargetActor = TargetActor;

			if (UEQRotationComponent* RotationComponent = Owner->GetComponentByClass<UEQRotationComponent>())
			{
				RotationComponent->SetTargetActor(TargetActor);
			}
		}
	}
	else
	{
		// 적합한 타겟 없으면 락온 종료
		StopLockOn();
	}
}

void UEQTargetingComponent::FindTargets(TArray<AActor*>& OutTargetingActors) const
{
	const FVector Start = Owner->GetActorLocation();
	const FVector End = Start;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(CCHANNEL_EQTARGET));

	const TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResults;

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(Owner, Start, End, TargetingRadius, ObjectTypes, false, ActorsToIgnore, DrawDebugType, HitResults, true);
	if (!bHit)
	{
		return;
	}

	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!HitActor)
		{
			continue;
		}

		if (!HitActor->Implements<UEQLockOnTargetable>())
		{
			continue;
		}

		IEQLockOnTargetable* LockOnTarget = Cast<IEQLockOnTargetable>(HitActor);
		if (!LockOnTarget)
		{
			continue;
		}

		if (!LockOnTarget->CanBeTargeted())
		{
			continue;
		}

		OutTargetingActors.Add(HitActor);
	}
}

AActor* UEQTargetingComponent::FindClosestTarget(TArray<AActor*>& InTargets, ESwitchingDirection InDirection) const
{
	float BestDot = -1.0f;
	AActor* ClosestTarget = nullptr;

	const FVector CameraLocation = CameraComponent->GetComponentLocation();
	const FVector RightVector = CameraComponent->GetRightVector();
	const FVector ForwardVector = CameraComponent->GetForwardVector();

	for (AActor* TargetActor : InTargets)
	{
		// 방향 스위칭 없고 기존 타겟은 제외
		if (InDirection == ESwitchingDirection::None && LockedTargetActor == TargetActor)
		{
			continue;
		}

		const FVector TargetLocation = TargetActor->GetActorLocation();
		TArray<AActor*> ActorsToIgnore;
		FHitResult HitResult;

		const bool bHit = UKismetSystemLibrary::LineTraceSingle(Owner, CameraLocation, TargetLocation, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, DrawDebugType, HitResult, true);
		if (!bHit || HitResult.GetActor() != TargetActor)
		{
			continue;
		}

		// 좌우 방향 필터링
		FVector ToTarget = (TargetLocation - CameraLocation).GetSafeNormal();
		const float SideDot = FVector::DotProduct(RightVector, ToTarget);

		if (InDirection == ESwitchingDirection::Left && SideDot >= 0.0f)
		{
			continue;
		}

		if (InDirection == ESwitchingDirection::Right && SideDot <= 0.0f)
		{
			continue;
		}

		// 정면 비교
		const float CheckValue = FVector::DotProduct(ForwardVector, ToTarget);
		if (CheckValue < 0.5f)
		{
			continue;
		}

		if (CheckValue > BestDot)
		{
			BestDot = CheckValue;
			ClosestTarget = TargetActor;
		}
	}

	// 좌우 스위칭 방향인데 대상이 없으면 기존 타겟 유지
	if ((InDirection == ESwitchingDirection::Left || InDirection == ESwitchingDirection::Right) && !ClosestTarget)
	{
		return LockedTargetActor;
	}

	return ClosestTarget;
}

void UEQTargetingComponent::LookAtLockedTarget() const
{
	// 현재 컨트롤러 회전 가져오기
	const FRotator CurrentControllerRotation = Owner->GetControlRotation();

	// 락온 타겟 위치 (Z축 보정 포함)
	constexpr float ZOffset = 100.0f;
	const FVector TargetLocation = LockedTargetActor->GetActorLocation() - FVector(0.0f, 0.0f, ZOffset);

	// 타겟을 바라보는 회전 계산
	const FRotator TargetLookAtRotation = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), TargetLocation);

	// 현재 회전에서 타겟 회전까지 부드럽게 보간
	const FRotator InterpRotation = FMath::RInterpTo(CurrentControllerRotation, TargetLookAtRotation, GetWorld()->GetDeltaSeconds(), TargetInterpSpeed);

	// 보간된 회전을 컨트롤러에 적용
	Owner->GetController()->SetControlRotation(FRotator(InterpRotation.Pitch, InterpRotation.Yaw, InterpRotation.Roll));
}

void UEQTargetingComponent::OrientCamera() const
{
	Owner->GetCharacterMovement()->bOrientRotationToMovement = false;
	Owner->GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void UEQTargetingComponent::OrientMovement() const
{
	Owner->GetCharacterMovement()->bOrientRotationToMovement = true;
	Owner->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void UEQTargetingComponent::StartLockOn()
{
	TArray<AActor*> TargetActors;
	FindTargets(TargetActors);

	AActor* TargetActor = FindClosestTarget(TargetActors);
	if (!TargetActor)
	{
		return;
	}

	LockedTargetActor = TargetActor;
	bIsLockOn = true;

	if (UEQRotationComponent* RotationComponent = Owner->GetComponentByClass<UEQRotationComponent>())
	{
		RotationComponent->SetTargetActor(TargetActor);
	}

	if (IEQLockOnTargetable* LockOnTarget = Cast<IEQLockOnTargetable>(LockedTargetActor))
	{
		LockOnTarget->OnTargeted(true);
		LockOnTarget->OnTargetOutOfHealth().AddDynamic(this, &ThisClass::OnLockedTargetOutOfHealth);
	}

	OrientCamera();
}

void UEQTargetingComponent::StopLockOn()
{
	bIsLockOn = false;

	if (IEQLockOnTargetable* LockOnTarget = Cast<IEQLockOnTargetable>(LockedTargetActor))
	{
		LockOnTarget->OnTargeted(false);
	}

	if (UEQRotationComponent* RotationComponent = Owner->GetComponentByClass<UEQRotationComponent>())
	{
		RotationComponent->SetTargetActor(nullptr);
	}

	LockedTargetActor = nullptr;
	OrientMovement();
}

void UEQTargetingComponent::OnLockedTargetOutOfHealth()
{
	StopLockOn();
}
