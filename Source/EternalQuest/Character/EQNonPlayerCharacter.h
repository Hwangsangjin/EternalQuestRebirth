// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EQBaseCharacter.h"
#include "Interface/EQLockOnTargetable.h"
#include "EQNonPlayerCharacter.generated.h"

class AEQItemBox;
class UEQRotationComponent;
class ATargetPoint;
class UWidgetComponent;
class USphereComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EAIBehavior : uint8
{
	Idle,
	Patrol,
	MeleeAttack,
	Approach
};

UCLASS()
class ETERNALQUEST_API AEQNonPlayerCharacter : public AEQBaseCharacter, public IEQLockOnTargetable
{
	GENERATED_BODY()

public:
	AEQNonPlayerCharacter();

	FORCEINLINE ATargetPoint* GetPatrolPoint() const { return PatrolPoints.Num() >= (PatrolIndex + 1) ? PatrolPoints[PatrolIndex] : nullptr; }
	FORCEINLINE void IncrementPatrolIndex() { PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num(); }

	UPROPERTY()
	FOnTargetOutOfHealthDelegate OnTargetOutOfHealthDelegate;

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PerformAttack(const FGameplayTag& AttackTypeTag, FOnMontageEnded& OnMontageEnded) override;
	virtual void Parried() override;
	virtual void OnTargeted(bool bTargeted) override;
	virtual bool CanBeTargeted() override;

	void InitAbilitySystem();
	void BindAttributeEvent();

	virtual FOnTargetOutOfHealthDelegate& OnTargetOutOfHealth() override;

	virtual void SetDead() override;

private:
	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQCharacterAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> InitStatEffectClass;

	UPROPERTY(EditAnywhere, Category = Component, Meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent> TargetingSphereComponent;

	UPROPERTY(EditAnywhere, Category = Component, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> LockOnWidgetComponent;

	UPROPERTY(EditAnywhere, Category = AI, Meta = (AllowPrivateAccess = true))
	TArray<ATargetPoint*> PatrolPoints;

	UPROPERTY(VisibleAnywhere, Category = AI, Meta = (AllowPrivateAccess = true))
	int32 PatrolIndex;

	UPROPERTY(EditAnywhere, Category = Item, Meta = (AllowPrivateAccess = true))
	TSubclassOf<AEQItemBox> ItemBoxClass;
};
