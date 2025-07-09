// Fill out your copyright notice in the Description page of Project Settings.

#include "EQCollisionComponent.h"

UEQCollisionComponent::UEQCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceRadius = 20.0f;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	DrawDebugType = EDrawDebugTrace::Type::ForDuration;
}

void UEQCollisionComponent::SetPrimitiveComponent(class UPrimitiveComponent* NewPrimitiveComponent)
{
	PrimitiveComponent = NewPrimitiveComponent;
}

void UEQCollisionComponent::TurnOnCollision()
{
	AlreadyHitActors.Empty();
	bIsCollisionEnabled = true;
}

void UEQCollisionComponent::TurnOffCollision()
{
	bIsCollisionEnabled = false;
}

void UEQCollisionComponent::AddIgnoredActor(AActor* Actor)
{
	IgnoreActors.Add(Actor);
}

void UEQCollisionComponent::RemoveIgnoredActor(AActor* Actor)
{
	IgnoreActors.Remove(Actor);
}

bool UEQCollisionComponent::CanHitActor(AActor* Actor) const
{
	return AlreadyHitActors.Contains(Actor) == false;
}

void UEQCollisionComponent::CollisionTrace()
{
	TArray<FHitResult> HitResults;

	const FVector Start = PrimitiveComponent->GetSocketLocation(StartSocketName);
	const FVector End = PrimitiveComponent->GetSocketLocation(EndSocketName);

	bool const bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(GetOwner(), Start, End, TraceRadius, ObjectTypes, false, IgnoreActors, DrawDebugType, HitResults, true);
	if (!bHit)
	{
		return;
	}

	for (const FHitResult& HitResult : HitResults)
	{
		AActor* HitActor = HitResult.GetActor();
		if (!HitActor || !CanHitActor(HitActor))
		{
			continue;
		}

		AlreadyHitActors.Add(HitActor);

		if (OnHitDelegate.IsBound())
		{
			OnHitDelegate.Broadcast(HitResult);
		}
	}
}

void UEQCollisionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCollisionEnabled)
	{
		CollisionTrace();
	}
}