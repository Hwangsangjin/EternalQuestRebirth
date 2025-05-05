// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAttributeComponent.h"

UEQAttributeComponent::UEQAttributeComponent()
{
	RegainRate = 1.0f;
	MaxStamina = 100.0f;
	BaseStamina = MaxStamina;
}

void UEQAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	ModifyStamina(MaxStamina);
}

void UEQAttributeComponent::ModifyStamina(float Delta)
{
	// 스태미나를 RegainRate만큼 회복하되, 최대치를 초과하지 않도록 클램프
	BaseStamina = FMath::Clamp(BaseStamina + Delta, 0.0f, MaxStamina);

	// 스태미나 변경 알림
	BroadcastAttributeChanged(EEQAttributeType::Stamina);
}

bool UEQAttributeComponent::CheckHasEnoughStamina(float StaminaCost) const
{
	return BaseStamina >= StaminaCost;
}

void UEQAttributeComponent::DecreaseStamina(float StaminaCost)
{
	ModifyStamina(-StaminaCost);
}

void UEQAttributeComponent::ToggleRegainStamina(bool bEnabled)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (bEnabled)
	{
		if (!TimerManager.IsTimerActive(RegainStaminaTimerHandle))
		{
			TimerManager.SetTimer(RegainStaminaTimerHandle, this, &ThisClass::RegainStamina, 0.1f, true);
		}
	}
	else
	{
		TimerManager.ClearTimer(RegainStaminaTimerHandle);
	}
}

void UEQAttributeComponent::RegainStamina()
{
	ModifyStamina(RegainRate);

	// 스태미나가 가득 찼다면 회복 중지
	if (BaseStamina >= MaxStamina)
	{
		ToggleRegainStamina(false);
	}
}

void UEQAttributeComponent::BroadcastAttributeChanged(EEQAttributeType InAttributeType) const
{
	// 델리게이트가 바인딩되어 있을 경우만 실행
	if (OnAttributeChanged.IsBound())
	{
		float Ratio = 0.0f;

		switch (InAttributeType)
		{
		case EEQAttributeType::Stamina:
			Ratio = GetStaminaRatio();
			break;
		}

		// 속성 타입과 비율을 브로드캐스트
		OnAttributeChanged.Broadcast(InAttributeType, Ratio);
	}
}
