// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interface/EQCombatCapable.h"
#include "EQBaseCharacter.generated.h"

class UEQInventoryComponent;
class UNiagaraSystem;
class USoundCue;
class UEQRotationComponent;
class UEQCombatComponent;
class UEQStateComponent;
class UEQAttributeComponent;
class UGameplayAbility;
class UEQCharacterAttributeSet;
class UEQWidgetComponent;
class UInputAction;
class UEQCharacterControlData;
class UEQComboActionData;

UENUM(BlueprintType)
enum class ECharacterControlType : uint8
{
	Default	UMETA(DisplayName = "Default")
};

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Front	UMETA(DisplayName = "Front"),
	Back	UMETA(DisplayName = "Back"),
	Left	UMETA(DisplayName = "Left"),
	Right	UMETA(DisplayName = "Right")
};

UENUM(BlueprintType)
enum class ECombatType : uint8
{
	None		UMETA(DisplayName = "None"),
	SwordShield	UMETA(DisplayName = "SwordShield"),
	TwoHanded	UMETA(DisplayName = "TwoHanded"),
};

UCLASS()
class ETERNALQUEST_API AEQBaseCharacter : public ACharacter, public IAbilitySystemInterface, public IEQCombatCapable
{
	GENERATED_BODY()

public:
	AEQBaseCharacter();

	FORCEINLINE UEQStateComponent* GetStateComponent() const { return StateComponent; }

	FORCEINLINE UAnimMontage* GetAttackMontage() const { return AttackMontage; }
	FORCEINLINE UEQComboActionData* GetComboActionData() const { return ComboActionData; }
	FORCEINLINE virtual UAnimMontage* GetSkillMontage() const { return SkillMontage; }
	FORCEINLINE virtual UAnimMontage* GetRollMontage() const { return RollMontage; }

	void HitReaction(const AActor* Attacker);

	bool CanPerformBlock(const AActor* Attacker) const;

	bool CanPerformParry() const;
	bool ParriedAttackSucceed() const;

	bool CanDrinkPotion() const;
	void InterruptWhileDrinkingPotion() const;

	bool IsDead() const;

protected:
	virtual void BeginPlay() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void SetCharacterControlData(const UEQCharacterControlData* CharacterControlData);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual AEQWeapon* GetWeapon() const override;
	virtual AEQShield* GetShield() const override;
	virtual bool IsCombatEnabled() const override;
	virtual void SetCombatEnabled(bool bEnabled) override;
	virtual void ActivateWeaponCollision() override;
	virtual void DeactivateWeaponCollision() override;

	bool CanBlockStance() const;

	void ImpactEffect(const AActor* Attacker, const FVector& Location);
	void ShieldBlockingEffect(const FVector& Location);

	UFUNCTION()
	virtual void SetDead();

	void HideHealthBarWidget();
	void PlayDeadAnimation();

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;

	UPROPERTY(VisibleAnywhere, Category = Component, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, Category = Component, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQStateComponent> StateComponent;

	UPROPERTY(VisibleAnywhere, Category = Component, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQCombatComponent> CombatComponent;

	UPROPERTY(EditAnywhere, Category = Component, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQRotationComponent> RotationComponent;

	UPROPERTY(EditAnywhere, Category = Component, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, Category = Component, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQWidgetComponent> HealthBarWidgetComponent;

	UPROPERTY(EditAnywhere, Category = CharacterControl, Meta = (AllowPrivateAccess = true))
	TMap<ECharacterControlType, UEQCharacterControlData*> CharacterControlMap;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> SkillMontage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> RollMontage;

	UPROPERTY(EditAnywhere, Category = Montage, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> DrinkMontage;

	UPROPERTY(EditAnywhere, Category = Combo, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQComboActionData> ComboActionData;

	UPROPERTY(EditAnywhere, Category = Combo, Meta = (AllowPrivateAccess = true))
	int32 CurrentComboIndex;

	UPROPERTY(EditAnywhere, Category = Combo, Meta = (AllowPrivateAccess = true))
	FTimerHandle ComboTimerHandle;

	UPROPERTY(EditAnywhere, Category = Combo, Meta = (AllowPrivateAccess = true))
	uint8 bHasNextComboCommand : 1;

	UPROPERTY(EditAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = true))
	TSubclassOf<AEQWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, Category = Weapon, Meta = (AllowPrivateAccess = true))
	TSubclassOf<AEQShield> ShieldClass;

	UPROPERTY(EditAnywhere, Category = Effect, Meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundCue> ImpactSoundCue;

	UPROPERTY(EditAnywhere, Category = Effect, Meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundCue> BlockingSoundCue;

	UPROPERTY(EditAnywhere, Category = Effect, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraSystem> ImpactNiagaraSystem;

	UPROPERTY(EditAnywhere, Category = Effect, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UNiagaraSystem> BlockingNiagaraSystem;

	UPROPERTY(EditAnywhere, Category = Value, Meta = (AllowPrivateAccess = true))
	uint8 bSprinting : 1;

	UPROPERTY(EditAnywhere, Category = Value, Meta = (AllowPrivateAccess = true))
	uint8 bFacing : 1;
};
