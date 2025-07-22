// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EQTargetActor_Trace.h"
#include "EQTargetActor_MultiTrace.generated.h"

UCLASS()
class ETERNALQUEST_API AEQTargetActor_MultiTrace : public AEQTargetActor_Trace
{
	GENERATED_BODY()

public:
	AEQTargetActor_MultiTrace();

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;
};
