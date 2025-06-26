// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EQEquipment.generated.h"

UCLASS()
class ETERNALQUEST_API AEQEquipment : public AActor
{
	GENERATED_BODY()

public:
	AEQEquipment();

	virtual void EquipItem();
	virtual void UnequipItem();
	virtual void AttachToOwner(FName SocketName);

	FORCEINLINE USkeletalMesh* GetSkeletalMesh() const { return SkeletalMesh; }
	FORCEINLINE void SetSkeletalMesh(USkeletalMesh* NewSkeletalMesh) { SkeletalMesh = NewSkeletalMesh; }

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, Category = Equipment, Meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = Equipment, Meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
};
