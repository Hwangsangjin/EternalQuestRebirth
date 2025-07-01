// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EQItemData.generated.h"

class AEQEquipment;

UCLASS()
class ETERNALQUEST_API UEQItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Equipment)
	TSubclassOf<AEQEquipment> EquipmentClass;
};
