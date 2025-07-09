// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EQTargetingComponent.generated.h"

class UCameraComponent;

namespace EDrawDebugTrace
{
	enum Type : int;
}

UENUM(BlueprintType)
enum class ESwitchingDirection : uint8
{
	None	UMETA(DisplayName = "None"),
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right")
};

UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class ETERNALQUEST_API UEQTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEQTargetingComponent();

	void ToggleLockOn();
	void SwitchingLockedOnActor(ESwitchingDirection InDirection);

	FORCEINLINE bool IsLockOn() const { return bIsLockOn; }

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FindTargets(OUT TArray<AActor*>& OutTargetingActors) const;

	AActor* FindClosestTarget(TArray<AActor*>& InTargets, ESwitchingDirection InDirection = ESwitchingDirection::None) const;

	void LookAtLockedTarget() const;

	void OrientCamera() const;
	void OrientMovement() const;

	void StartLockOn();
	void StopLockOn();

	UFUNCTION()
	void OnLockedTargetOutOfHealth();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	float TargetingRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	float TargetInterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, Meta = (AllowPrivateAccess = true))
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

	UPROPERTY()
	TObjectPtr<ACharacter> Owner;

	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY()
	TObjectPtr<AActor> LockedTargetActor;

	UPROPERTY()
	uint8 bIsLockOn : 1;
};
