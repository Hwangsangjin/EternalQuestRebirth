// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "EQCharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)	   \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)			   \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)			   \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfHealthDelegate);

class UEQGameplayEffect_AttackDamage;

UCLASS()
class ETERNALQUEST_API UEQCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UEQCharacterAttributeSet();

	ATTRIBUTE_ACCESSORS(UEQCharacterAttributeSet, Damage)
	ATTRIBUTE_ACCESSORS(UEQCharacterAttributeSet, AttackRange)
	ATTRIBUTE_ACCESSORS(UEQCharacterAttributeSet, MaxAttackRange)
	ATTRIBUTE_ACCESSORS(UEQCharacterAttributeSet, AttackRadius)
	ATTRIBUTE_ACCESSORS(UEQCharacterAttributeSet, MaxAttackRadius)
	ATTRIBUTE_ACCESSORS(UEQCharacterAttributeSet, AttackRate)
	ATTRIBUTE_ACCESSORS(UEQCharacterAttributeSet, MaxAttackRate)
	ATTRIBUTE_ACCESSORS(UEQCharacterAttributeSet, Health)
	ATTRIBUTE_ACCESSORS(UEQCharacterAttributeSet, MaxHealth)
	ATTRIBUTE_ACCESSORS(UEQCharacterAttributeSet, Mana)
	ATTRIBUTE_ACCESSORS(UEQCharacterAttributeSet, MaxMana)
	ATTRIBUTE_ACCESSORS(UEQCharacterAttributeSet, Stamina)
	ATTRIBUTE_ACCESSORS(UEQCharacterAttributeSet, MaxStamina)

	UPROPERTY()
	mutable FOnOutOfHealthDelegate OnOutOfHealth;

protected:
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRange;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRange;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRadius;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackRate;

	UPROPERTY(BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxAttackRate;

	UPROPERTY(BlueprintReadOnly, Category = Health, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category = Health, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = Health, Meta = (AllowPrivateAccess = true))
	uint8 bOutOfHealth : 1;

	UPROPERTY(BlueprintReadOnly, Category = Mana, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Mana;

	UPROPERTY(BlueprintReadOnly, Category = Mana, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxMana;

	UPROPERTY(BlueprintReadOnly, Category = Stamina, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	UPROPERTY(BlueprintReadOnly, Category = Stamina, Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;

	friend UEQGameplayEffect_AttackDamage;
};
