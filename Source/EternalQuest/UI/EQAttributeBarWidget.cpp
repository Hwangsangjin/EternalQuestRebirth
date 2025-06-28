// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAttributeBarWidget.h"
#include "AbilitySystemComponent.h"
#include "Attribute/EQCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameplayTag/EQGameplayTag.h"

UEQAttributeBarWidget::UEQAttributeBarWidget()
{
	CurrentValue = 0.0f;
	MaxValue = 0.1f;
	LastPercent = 1.0f;
	DelayTimer = 0.0f;
	BackBarLerpDelay = 0.5f;
}

void UEQAttributeBarWidget::SetAttributeByType()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	const UEQCharacterAttributeSet* AttributeSet = AbilitySystemComponent->GetSet<UEQCharacterAttributeSet>();
	if (!AttributeSet)
	{
		return;
	}

	switch (AttributeType)
	{
	case EAttributeType::Health:
		BindAttributePair(UEQCharacterAttributeSet::GetHealthAttribute(), UEQCharacterAttributeSet::GetMaxHealthAttribute());
		break;
	case EAttributeType::Mana:
		BindAttributePair(UEQCharacterAttributeSet::GetManaAttribute(), UEQCharacterAttributeSet::GetMaxManaAttribute());
		break;
	case EAttributeType::Stamina:
		BindAttributePair(UEQCharacterAttributeSet::GetStaminaAttribute(), UEQCharacterAttributeSet::GetMaxStaminaAttribute());
		break;
	}
}

void UEQAttributeBarWidget::BindAttributePair(const FGameplayAttribute& CurrentAttribute, const FGameplayAttribute& MaxAttribute)
{
	AttributeToBind = CurrentAttribute;
	MaxAttributeToBind = MaxAttribute;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CurrentAttribute).AddUObject(this, &ThisClass::OnAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxAttribute).AddUObject(this, &ThisClass::OnMaxAttributeChanged);

	CurrentValue = AbilitySystemComponent->GetNumericAttribute(CurrentAttribute);
	MaxValue = AbilitySystemComponent->GetNumericAttribute(MaxAttribute);
	UpdateProgressBar(CurrentValue, MaxValue);
}

void UEQAttributeBarWidget::UpdateProgressBar(float InCurrentValue, float InMaxValue)
{
	check(FrontProgressBar);
	check(BackProgressBar);
	check(TextBlock);

	// 비율 계산
	CurrentValue = InCurrentValue;
	MaxValue = InMaxValue;
	const float Percent = (InMaxValue > 0.0f) ? (InCurrentValue / InMaxValue) : 0.0f;

	FrontProgressBar->SetPercent(Percent);
	TextBlock->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), CurrentValue, MaxValue)));

	const bool bIsSprinting = AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(EQGameplayTag::Character_State_Sprinting);
	if (bIsSprinting)
	{
		// 스프린트 중이면 즉시 반영
		BackProgressBar->SetPercent(Percent);
	}
	else if (Percent < LastPercent)
	{
		// 스프린트 중이 아니고 감소 → 지연 후 보간
		DelayTimer = BackBarLerpDelay;
		bShouldLerp = false;
	}

	// 마지막 비율 저장
	LastPercent = Percent;
}

void UEQAttributeBarWidget::SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent)
{
	Super::SetAbilitySystemComponent(InAbilitySystemComponent);

	if (!AbilitySystemComponent)
	{
		return;
	}

	AbilitySystemComponent->RegisterGameplayTagEvent(EQGameplayTag::Character_State_Blocking, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnInvisibleTagChanged);

	check(FrontProgressBar);
	FrontProgressBar->SetFillColorAndOpacity(NormalColor);

	SetAttributeByType();
}

void UEQAttributeBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	check(FrontProgressBar);
	check(BackProgressBar);

	// 현재 BackProgressBar의 퍼센트 (느리게 따라오는 바)
	float A = BackProgressBar->GetPercent();
	// FrontProgressBar의 퍼센트 (즉시 반영되는 바)
	const float B = FrontProgressBar->GetPercent();

	if (bShouldLerp)
	{
		// 부드럽게 A를 B로 보간
		A = FMath::FInterpTo(A, B, InDeltaTime, InterpSpeed);
		BackProgressBar->SetPercent(A);

		// 보간이 거의 끝났으면 보간 종료 처리
		if (FMath::IsNearlyEqual(A, B, KINDA_SMALL_NUMBER))
		{
			BackProgressBar->SetPercent(B);
			bShouldLerp = false;
		}
	}
	else
	{
		// 보간 대기 시간 감소
		DelayTimer -= InDeltaTime;
		if (DelayTimer <= 0.0f)
		{
			// 대기 시간이 끝나면 보간 시작
			bShouldLerp = true;
		}
	}
}

void UEQAttributeBarWidget::OnAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentValue = ChangeData.NewValue;
	UpdateProgressBar(CurrentValue, MaxValue);
}

void UEQAttributeBarWidget::OnMaxAttributeChanged(const FOnAttributeChangeData& ChangeData)
{
	MaxValue = ChangeData.NewValue;
	UpdateProgressBar(CurrentValue, MaxValue);
}

void UEQAttributeBarWidget::OnInvisibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FrontProgressBar->SetFillColorAndOpacity(InvisibleColor);
	}
	else
	{
		FrontProgressBar->SetFillColorAndOpacity(NormalColor);
	}
}
