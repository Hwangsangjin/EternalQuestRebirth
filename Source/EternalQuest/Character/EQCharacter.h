// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EQCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UEQAttributeComponent;

UCLASS()
class ETERNALQUEST_API AEQCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEQCharacter();

protected:
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	bool IsMoving() const;
	void Sprinting();
	void StopSprint();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQAttributeComponent> AttributeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Value, meta = (AllowPrivateAccess = true))
	float MovingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Value, meta = (AllowPrivateAccess = true))
	float StaminaCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Value, meta = (AllowPrivateAccess = true))
	float SprintCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Value, meta = (AllowPrivateAccess = true))
	float SprintSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Value, meta = (AllowPrivateAccess = true))
	float NormalSpeed;
};
