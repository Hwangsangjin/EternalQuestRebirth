// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Blueprint/UserWidget.h"
#include "EQUserWidget.generated.h"

UCLASS()
class ETERNALQUEST_API UEQUserWidget : public UUserWidget, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	explicit UEQUserWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent);

protected:
	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
