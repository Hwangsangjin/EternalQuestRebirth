// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EQPlayerController.generated.h"

class UEQHUDWidget;

UCLASS()
class ETERNALQUEST_API AEQPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AEQPlayerController();

	FORCEINLINE UEQHUDWidget* GetHUDWidget() const { return HUDWidget; }

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UEQHUDWidget> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = UI, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQHUDWidget> HUDWidget;
};
