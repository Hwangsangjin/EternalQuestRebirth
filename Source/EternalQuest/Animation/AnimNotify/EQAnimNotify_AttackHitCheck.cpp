// Fill out your copyright notice in the Description page of Project Settings.

#include "EQAnimNotify_AttackHitCheck.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTag/EQGameplayTag.h"

UEQAnimNotify_AttackHitCheck::UEQAnimNotify_AttackHitCheck()
{
	TriggerGameplayTag = EQGameplayTag::Character_Action_AttackHitCheck;

	ComboLevel = 1.0f;
}

FString UEQAnimNotify_AttackHitCheck::GetNotifyName_Implementation() const
{
	return TEXT("AttackHitCheck");
}

void UEQAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp)
	{
		return;
	}

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
	{
		return;
	}

	FGameplayEventData PayloadData;
	PayloadData.Instigator = Owner;
	PayloadData.EventMagnitude = ComboLevel;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, TriggerGameplayTag, PayloadData);
}
