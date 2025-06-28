// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EQUserWidget.h"
#include "EQButtonWidget.generated.h"

struct FGameplayTag;

UCLASS()
class ETERNALQUEST_API UEQButtonWidget : public UEQUserWidget
{
	GENERATED_BODY()

public:
	UEQButtonWidget();

	virtual void SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent) override;

protected:
	UFUNCTION()
	void OnButtonPressed(const FGameplayTag Tag, int32 NewCount);

private:
	UPROPERTY(EditAnywhere, Transient, Meta = (BindWidgetAnim, AllowPrivateAccess = true))
	UWidgetAnimation* PressedAnim;
};
