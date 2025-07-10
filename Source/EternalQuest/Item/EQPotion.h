// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EQPotion.generated.h"

UCLASS()
class ETERNALQUEST_API AEQPotion : public AActor
{
	GENERATED_BODY()

public:
	AEQPotion();

protected:

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
};
