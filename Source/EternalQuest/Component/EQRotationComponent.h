// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EQRotationComponent.generated.h"

UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class ETERNALQUEST_API UEQRotationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEQRotationComponent();

	FORCEINLINE void SetTargetActor(AActor* InActor) { TargetActor = InActor; }
	FORCEINLINE void ToggleShouldRotate(const bool bRotate) { bShouldRotate = bRotate; }

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Target, Meta = (AllowPrivateAccess = true))
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Target, Meta = (AllowPrivateAccess = true))
	uint8 bShouldRotate : 1;
};
