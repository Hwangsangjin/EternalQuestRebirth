// Fill out your copyright notice in the Description page of Project Settings.

#include "EQPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Attribute/EQCharacterAttributeSet.h"
#include "Attribute/EQCharacterSkillAttributeSet.h"

AEQPlayerState::AEQPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UEQCharacterAttributeSet>(TEXT("AttributeSet"));
	SkillAttributeSet = CreateDefaultSubobject<UEQCharacterSkillAttributeSet>(TEXT("SkillAttributeSet"));
}

UAbilitySystemComponent* AEQPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
