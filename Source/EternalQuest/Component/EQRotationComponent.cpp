// Fill out your copyright notice in the Description page of Project Settings.

#include "EQRotationComponent.h"
#include "Kismet/KismetMathLibrary.h"

UEQRotationComponent::UEQRotationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEQRotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!TargetActor)
	{
		return;
	}

	if (!bShouldRotate)
	{
		return;
	}

	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation());
	GetOwner()->SetActorRotation(FRotator(0.0f, LookAtRotation.Yaw, 0.0f));
}
