// Fill out your copyright notice in the Description page of Project Settings.

#include "EQStatBarWidget.h"
#include "Components/ProgressBar.h"

UEQStatBarWidget::UEQStatBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UEQStatBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ProgressBar == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Progress Bar is nullptr."));
		return;
	}

	ProgressBar->SetFillColorAndOpacity(FillColorAndOpacity);
}

void UEQStatBarWidget::SetProgressBarPercent(float InRatio) const
{
	if (ProgressBar == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Progress Bar is nullptr."));
		return;
	}

	ProgressBar->SetPercent(InRatio);
}
