// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EQUserWidget.h"
#include "EQHUDWidget.generated.h"

class UEQButtonWidget;
class UEQAttributeBarWidget;

UCLASS()
class ETERNALQUEST_API UEQHUDWidget : public UEQUserWidget
{
	GENERATED_BODY()

public:
	explicit UEQHUDWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	FORCEINLINE UEQUserWidget* GetInteractPromptWidget() const { return InteractPromptWidget; }
	FORCEINLINE UEQUserWidget* GetWarningPromptWidget() const { return WarningPromptWidget; }

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UEQAttributeBarWidget> HealthBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UEQAttributeBarWidget> ManaBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UEQAttributeBarWidget> StaminaBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UEQUserWidget> InteractPromptWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UEQUserWidget> WarningPromptWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (BindWidget, AllowPrivateAccess = true))
	TObjectPtr<UEQButtonWidget> LeftButtonWidget;
};
