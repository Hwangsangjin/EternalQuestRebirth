// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "EQGameplayCueNotify_WarningPrompt.generated.h"

UCLASS()
class ETERNALQUEST_API UEQGameplayCueNotify_WarningPrompt : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UEQGameplayCueNotify_WarningPrompt();

protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
