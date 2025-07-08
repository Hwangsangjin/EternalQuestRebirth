// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "EQCharacterSkillAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)	   \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)			   \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)			   \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class ETERNALQUEST_API UEQCharacterSkillAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEQCharacterSkillAttributeSet();

	ATTRIBUTE_ACCESSORS(UEQCharacterSkillAttributeSet, SkillRange)
	ATTRIBUTE_ACCESSORS(UEQCharacterSkillAttributeSet, MaxSkillRange)
	ATTRIBUTE_ACCESSORS(UEQCharacterSkillAttributeSet, SkillRate)
	ATTRIBUTE_ACCESSORS(UEQCharacterSkillAttributeSet, MaxSkillRate)
	ATTRIBUTE_ACCESSORS(UEQCharacterSkillAttributeSet, SkillEnergy)
	ATTRIBUTE_ACCESSORS(UEQCharacterSkillAttributeSet, MaxSkillEnergy)

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillRange;

	UPROPERTY(BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillRange;

	UPROPERTY(BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillRate;

	UPROPERTY(BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillRate;

	UPROPERTY(BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData SkillEnergy;

	UPROPERTY(BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxSkillEnergy;
};
