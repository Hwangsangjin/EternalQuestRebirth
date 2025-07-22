// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EQTargetActor_Trace.h"
#include "EQTargetActor_InteractTrace.generated.h"

UCLASS()
class ETERNALQUEST_API AEQTargetActor_InteractTrace : public AEQTargetActor_Trace
{
	GENERATED_BODY()

public:
	AEQTargetActor_InteractTrace();

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;

protected:
	virtual FGameplayAbilityTargetDataHandle MakeTargetData() const override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trace, Meta = (AllowPrivateAccess = true))
	TEnumAsByte<EObjectTypeQuery> ObjectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trace, Meta = (AllowPrivateAccess = true))
	float TraceRadius;
};
