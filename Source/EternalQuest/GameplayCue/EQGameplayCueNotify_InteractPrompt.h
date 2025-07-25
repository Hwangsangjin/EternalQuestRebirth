// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Static.h"
#include "EQGameplayCueNotify_InteractPrompt.generated.h"

UCLASS()
class ETERNALQUEST_API UEQGameplayCueNotify_InteractPrompt : public UGameplayCueNotify_Static
{
	GENERATED_BODY()

public:
	UEQGameplayCueNotify_InteractPrompt();

protected:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) const override;
};
