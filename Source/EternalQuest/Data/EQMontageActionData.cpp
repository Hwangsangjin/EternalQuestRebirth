// Fill out your copyright notice in the Description page of Project Settings.

#include "EQMontageActionData.h"

UEQMontageActionData::UEQMontageActionData()
{
}

UAnimMontage* UEQMontageActionData::GetMontageForTag(const FGameplayTag& GameplayTag, const int32 Index) const
{
	const FEQMontageGroup* MontageGroup = MontageGroupMap.Find(GameplayTag);
	if (!MontageGroup)
	{
		return nullptr;
	}

	if (MontageGroup->AnimMontages.IsValidIndex(Index))
	{
		return MontageGroup->AnimMontages[Index];
	}

	return nullptr;
}

UAnimMontage* UEQMontageActionData::GetRandomMontageForTag(const FGameplayTag& GameplayTag) const
{
	const FEQMontageGroup* MontageGroup = MontageGroupMap.Find(GameplayTag);
	if (!MontageGroup)
	{
		return nullptr;
	}

	const int32 RandomIndex = FMath::RandRange(0, MontageGroup->AnimMontages.Num() - 1);

	if (MontageGroup->AnimMontages.IsValidIndex(RandomIndex))
	{
		return GetMontageForTag(GameplayTag, RandomIndex);
	}

	return nullptr;
}
