// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EQUserWidget.generated.h"

UCLASS()
class ETERNALQUEST_API UEQUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE AActor* GetOwningActor() const { return OwningActor.Get(); }
	FORCEINLINE void SetOwningActor(AActor* InActor) { OwningActor = InActor; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Actor)
	TObjectPtr<AActor> OwningActor;
};
