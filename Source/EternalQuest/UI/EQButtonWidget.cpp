// Fill out your copyright notice in the Description page of Project Settings.

#include "EQButtonWidget.h"
#include "AbilitySystemComponent.h"
#include "GameplayTag/EQGameplayTag.h"

UEQButtonWidget::UEQButtonWidget()
{
}

void UEQButtonWidget::SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent)
{
	Super::SetAbilitySystemComponent(InAbilitySystemComponent);

	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->RegisterGameplayTagEvent(EQGameplayTag::UI_Notify_ButtonPressed, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnButtonPressed);
}

void UEQButtonWidget::OnButtonPressed(const FGameplayTag Tag, int32 NewCount)
{
	check(PressedAnim);

	if (NewCount > 0)
	{
		FTimerHandle DelayAnimTimer;
		GetWorld()->GetTimerManager().SetTimer(DelayAnimTimer, FTimerDelegate::CreateLambda([this]()
		{
			PlayAnimation(PressedAnim);
		}), 0.01f, false);
	}
}
