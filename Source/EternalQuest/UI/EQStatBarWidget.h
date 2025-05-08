// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EQUserWidget.h"
#include "EQStatBarWidget.generated.h"

class UProgressBar;

UCLASS()
class ETERNALQUEST_API UEQStatBarWidget : public UEQUserWidget
{
	GENERATED_BODY()

public:
	explicit UEQStatBarWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void SetProgressBarPercent(float InRatio) const;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UProgressBar> ProgressBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (AllowPrivateAccess = true))
	FLinearColor FillColorAndOpacity;
};
