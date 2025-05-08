// Fill out your copyright notice in the Description page of Project Settings.

#include "EQPlayerHUDWidget.h"
#include "EQStatBarWidget.h"
#include "Component/EQAttributeComponent.h"

UEQPlayerHUDWidget::UEQPlayerHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UEQPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const APawn* OwningPlayerPawn = GetOwningPlayerPawn();
	if (OwningPlayerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Owning Player Pawn is nullptr."));
		return;
	}

	UEQAttributeComponent* AttributeComponent = OwningPlayerPawn->FindComponentByClass<UEQAttributeComponent>();
	if (AttributeComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Attribute Component is nullptr."));
		return;
	}

	AttributeComponent->OnAttributeChanged.AddUObject(this, &ThisClass::UpdateWidget);
}

void UEQPlayerHUDWidget::UpdateWidget(EEQAttributeType InAttributeType, float InRatio)
{
	switch (InAttributeType)
	{
	case EEQAttributeType::Stamina:
		StaminaBarWidget->SetProgressBarPercent(InRatio);
		break;
	}
}
