// Fill out your copyright notice in the Description page of Project Settings.

#include "EQUserWidget.h"
#include "AbilitySystemBlueprintLibrary.h"

UEQUserWidget::UEQUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UAbilitySystemComponent* UEQUserWidget::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void UEQUserWidget::SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent)
{
	if (!InAbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent = InAbilitySystemComponent;
}
