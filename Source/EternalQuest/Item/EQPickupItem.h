// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interface/EQCharacterInteractable.h"
#include "EQPickupItem.generated.h"

class UGameplayEffect;
class UEQItemData;
class AEQEquipment;
class UBoxComponent;

UCLASS()
class ETERNALQUEST_API AEQPickupItem : public AActor, public IAbilitySystemInterface, public IEQCharacterInteractable
{
	GENERATED_BODY()

public:
	AEQPickupItem();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void PerformInteract(AActor* InteractActor) override;

	FORCEINLINE void SetEquipmentClass(const TSubclassOf<AEQEquipment>& NewEquipmentClass) { EquipmentClass = NewEquipmentClass; }

	void SetItemData(UEQItemData* NewItemData);
	void InvokeGameplayCue();
	void Throw();

protected:
	virtual void PostInitializeComponents() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = GAS, Meta = (AllowPrivateAccess = true))
	FGameplayTag GameplayCueTag;

	UPROPERTY(EditAnywhere, Category = Item, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UEQItemData> ItemData;

	UPROPERTY(VisibleAnywhere, Category = Box, Meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> TriggerBoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, Meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment, Meta = (AllowPrivateAccess = true))
	TSubclassOf<AEQEquipment> EquipmentClass;

	FTimerHandle LaunchTimerHandle;
};
