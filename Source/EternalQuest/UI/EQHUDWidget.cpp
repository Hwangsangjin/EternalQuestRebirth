// Fill out your copyright notice in the Description page of Project Settings.

#include "EQHUDWidget.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "EQAttributeBarWidget.h"
#include "EQButtonWidget.h"
#include "Components/TextBlock.h"

UEQHUDWidget::UEQHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UEQHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APawn* OwningPlayerPawn = GetOwningPlayerPawn();
	if (!OwningPlayerPawn)
	{
		return;
	}

	UAbilitySystemComponent* NewAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningPlayerPawn);
	if (!NewAbilitySystemComponent)
	{
		return;
	}

	check(HealthBarWidget);
	check(ManaBarWidget);
	check(StaminaBarWidget);
	check(LeftButtonWidget);

	HealthBarWidget->SetAbilitySystemComponent(NewAbilitySystemComponent);
	ManaBarWidget->SetAbilitySystemComponent(NewAbilitySystemComponent);
	StaminaBarWidget->SetAbilitySystemComponent(NewAbilitySystemComponent);
	LeftButtonWidget->SetAbilitySystemComponent(NewAbilitySystemComponent);

	check(InteractPromptWidget);
	check(WarningPromptWidget);

	InteractPromptWidget->SetVisibility(ESlateVisibility::Hidden);
	WarningPromptWidget->SetVisibility(ESlateVisibility::Hidden);
}
