// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "EQWidgetComponent.generated.h"

UCLASS(ClassGroup = (Custom), Meta = (BlueprintSpawnableComponent))
class ETERNALQUEST_API UEQWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UEQWidgetComponent();

protected:
	virtual void InitWidget() override;
};
