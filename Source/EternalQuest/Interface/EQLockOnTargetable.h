// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EQLockOnTargetable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetOutOfHealthDelegate);

UINTERFACE()
class UEQLockOnTargetable : public UInterface
{
	GENERATED_BODY()
};

class ETERNALQUEST_API IEQLockOnTargetable
{
	GENERATED_BODY()

public:
	virtual void OnTargeted(bool bTargeted) = 0;
	virtual bool CanBeTargeted() = 0;
	virtual FOnTargetOutOfHealthDelegate& OnTargetOutOfHealth() = 0;
};
