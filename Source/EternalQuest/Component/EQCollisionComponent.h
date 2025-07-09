// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EQCollisionComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitDelegate, const FHitResult&)

UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class ETERNALQUEST_API UEQCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEQCollisionComponent();

	void SetPrimitiveComponent(class UPrimitiveComponent* NewPrimitiveComponent);

	void TurnOnCollision();
	void TurnOffCollision();

	void AddIgnoredActor(AActor* Actor);
	void RemoveIgnoredActor(AActor* Actor);

	bool CanHitActor(AActor* Actor) const;

	void CollisionTrace();

	FOnHitDelegate OnHitDelegate;

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	FName StartSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	FName EndSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	float TraceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	TArray<AActor*> IgnoreActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UPrimitiveComponent> PrimitiveComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	TArray<AActor*> AlreadyHitActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision, Meta = (AllowPrivateAccess = true))
	uint8 bIsCollisionEnabled;
};
