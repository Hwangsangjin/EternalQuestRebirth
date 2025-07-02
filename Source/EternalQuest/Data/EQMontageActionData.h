// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "EQMontageActionData.generated.h"

USTRUCT(BlueprintType)
struct FEQMontageGroup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	TArray<UAnimMontage*> AnimMontages;
};

UCLASS()
class ETERNALQUEST_API UEQMontageActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UEQMontageActionData();

	UAnimMontage* GetMontageForTag(const FGameplayTag& GameplayTag, const int32 Index) const;
	UAnimMontage* GetRandomMontageForTag(const FGameplayTag& GameplayTag) const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage, Meta = (AllowPrivateAccess = true))
	TMap<FGameplayTag, FEQMontageGroup> MontageGroupMap;
};
