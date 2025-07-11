// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EQCharacterInteractable.generated.h"

UINTERFACE()
class UEQCharacterInteractable : public UInterface
{
	GENERATED_BODY()
};

class ETERNALQUEST_API IEQCharacterInteractable
{
	GENERATED_BODY()

public:
	virtual void PerformInteract(AActor* InteractActor) = 0;
};
