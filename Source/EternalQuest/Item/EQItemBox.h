// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interface/EQCharacterInteractable.h"
#include "EQItemBox.generated.h"

class UWidgetComponent;
class UEQInteractPromptWidget;
class UGameplayEffect;
class UEQItemData;
class UBoxComponent;

UCLASS()
class ETERNALQUEST_API AEQItemBox : public AActor, public IAbilitySystemInterface, public IEQCharacterInteractable
{
	GENERATED_BODY()

public:
	AEQItemBox();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PerformInteract(AActor* InteractActor) override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	void ApplyEffectToTarget(AActor* TargetActor);
	void InvokeGameplayCue(AActor* TargetActor);

private:
	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category = Box, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> TriggerBoxComponent;

	UPROPERTY(VisibleAnywhere, Category = Box, Meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	// UPROPERTY(EditAnywhere, Category = Item, Meta = (AllowPrivateAccess = true))
	// TObjectPtr<UEQItemData> ItemData;

	UPROPERTY(EditAnywhere, Category = Item, Meta = (AllowPrivateAccess = true))
	TArray<UEQItemData*> ItemDataArray;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	FGameplayTag GameplayCueTag;
};
