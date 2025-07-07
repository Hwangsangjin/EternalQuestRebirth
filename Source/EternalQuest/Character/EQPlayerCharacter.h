// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EQBaseCharacter.h"
#include "EQPlayerCharacter.generated.h"

class UGameplayEffect;
class UEQTargetingComponent;
class UEQInteractPromptWidget;
class UWidgetComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UEQAttributeComponent;
class UEQStateComponent;
class UEQCombatComponent;

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	Attack			UMETA(DisplayName = "Attack"),
	Interact		UMETA(DisplayName = "Interact"),
	Jump			UMETA(DisplayName = "Jump"),
	Roll			UMETA(DisplayName = "Roll"),
	Skill			UMETA(DisplayName = "Skill"),
	Sprint			UMETA(DisplayName = "Sprint"),
	ToggleCombat	UMETA(DisplayName = "ToggleCombat")
};

UCLASS()
class ETERNALQUEST_API AEQPlayerCharacter : public AEQBaseCharacter
{
	GENERATED_BODY()

public:
	AEQPlayerCharacter();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	void SetCharacterControl(ECharacterControlType NewCharacterControlType);
	virtual void SetCharacterControlData(const UEQCharacterControlData* CharacterControlData) override;

	void InitAbilitySystem();
	void BindAttributeEvent();
	void SetupInputComponent();
	void InputPressed(EAbilityType AbilityType);
	void InputReleased(EAbilityType AbilityType);

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void LockOnTarget();
	void LeftTarget();
	void RightTarget();

	void Blocking();
	void BlockingEnd();

	void Parrying();

	void Consume();

private:
	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TMap<int32, TSubclassOf<UGameplayAbility>> StartInputAbilities;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayAbility> StaminaRegenAbilityClass;

	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, Category = Camera, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = Component, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQTargetingComponent> TargetingComponent;

	UPROPERTY(VisibleAnywhere, Category = Control, Meta = (AllowPrivateAccess = true))
	ECharacterControlType CharacterControlType;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SprintAndRollAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> RollAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> ToggleCombatAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> AttackAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SkillAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> LockOnTargetAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> LeftTargetAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> RightTargetAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> BlockAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> ParryAction;

	UPROPERTY(EditAnywhere, Category = Input, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> ConsumeAction;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> PotionHealEffect;
};
