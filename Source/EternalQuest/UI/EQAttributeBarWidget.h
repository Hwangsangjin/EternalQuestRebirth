// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "EQUserWidget.h"
#include "EQAttributeBarWidget.generated.h"

struct FGameplayTag;
struct FOnAttributeChangeData;
class UTextBlock;
class UProgressBar;

UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	Health	UMETA(DisplayName = "Health"),
	Mana	UMETA(DisplayName = "Mana"),
	Stamina	UMETA(DisplayName = "Stamina")
};

UCLASS()
class ETERNALQUEST_API UEQAttributeBarWidget : public UEQUserWidget
{
	GENERATED_BODY()

public:
	UEQAttributeBarWidget();

	virtual void SetAbilitySystemComponent(UAbilitySystemComponent* InAbilitySystemComponent) override;

	void SetAttributeByType();
	void BindAttributePair(const FGameplayAttribute& CurrentAttribute, const FGameplayAttribute& MaxAttribute);

	void UpdateProgressBar(float InCurrentValue, float InMaxValue);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void OnAttributeChanged(const FOnAttributeChangeData& ChangeData);
	void OnMaxAttributeChanged(const FOnAttributeChangeData& ChangeData);
	void OnInvisibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attribute, Meta = (AllowPrivateAccess = true))
	EAttributeType AttributeType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attribute, Meta = (AllowPrivateAccess = true))
	FGameplayAttribute AttributeToBind;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attribute, Meta = (AllowPrivateAccess = true))
	FGameplayAttribute MaxAttributeToBind;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attribute, Meta = (AllowPrivateAccess = true))
	float CurrentValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attribute, Meta = (AllowPrivateAccess = true))
	float MaxValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> FrontProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> BackProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UTextBlock> TextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
	float DelayTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
	float LastPercent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
	uint8 bShouldLerp : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
	float BackBarLerpDelay = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
	float InterpSpeed = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
	FLinearColor NormalColor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
	FLinearColor InvisibleColor;
};
