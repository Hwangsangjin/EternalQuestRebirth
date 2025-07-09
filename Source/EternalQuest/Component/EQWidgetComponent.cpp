// Fill out your copyright notice in the Description page of Project Settings.

#include "EQWidgetComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "UI/EQUserWidget.h"

UEQWidgetComponent::UEQWidgetComponent()
{
}

void UEQWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UAbilitySystemComponent* NewAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (!NewAbilitySystemComponent)
	{
		return;
	}

	UEQUserWidget* EQUserWidget = Cast<UEQUserWidget>(GetWidget());
	if (!EQUserWidget)
	{
		return;
	}

	EQUserWidget->SetAbilitySystemComponent(NewAbilitySystemComponent);
}
