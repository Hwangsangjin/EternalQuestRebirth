// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EQUserWidget.h"
#include "EternalQuestDefine.h"
#include "EQPlayerHUDWidget.generated.h"

class UEQStatBarWidget;

UCLASS()
class ETERNALQUEST_API UEQPlayerHUDWidget : public UEQUserWidget
{
	GENERATED_BODY()

public:
	explicit UEQPlayerHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void UpdateWidget(EEQAttributeType InAttributeType, float InRatio);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UEQStatBarWidget> StaminaBarWidget;
};
