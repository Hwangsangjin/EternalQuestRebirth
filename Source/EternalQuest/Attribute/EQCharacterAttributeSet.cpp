// Fill out your copyright notice in the Description page of Project Settings.

#include "EQCharacterAttributeSet.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffectExtension.h"
#include "GameplayAbility/EQGameplayAbility_Sprint.h"
#include "GameplayTag/EQGameplayTag.h"

UEQCharacterAttributeSet::UEQCharacterAttributeSet()
	: Damage(0.0f)
	, AttackRange(100.0f)
	, MaxAttackRange(300.0f)
	, AttackRadius(50.0f)
	, MaxAttackRadius(150.0f)
	, AttackRate(30.0f)
	, MaxAttackRate(100.0f)
	, MaxHealth(100.0f)
	, MaxMana(100.0f)
	, MaxStamina(100.0f)
{
	InitHealth(GetMaxHealth());
	InitMana(GetMaxMana());
	InitStamina(GetMaxStamina());
}

void UEQCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// if (Attribute == GetHealthAttribute())
	// {
	// 	NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	// }

	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}

	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}

	if (Attribute == GetDamageAttribute())
	{
		NewValue = NewValue < 0.0f ? 0.0f : NewValue;
	}
}

bool UEQCharacterAttributeSet::PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			if (Data.Target.HasMatchingGameplayTag(EQGameplayTag::Character_State_Invincible))
			{
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}
	}

	return true;
}

void UEQCharacterAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	constexpr float MinimumValue = 0.0f;

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), MinimumValue, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), MinimumValue, GetMaxMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), MinimumValue, GetMaxStamina()));

		if (GetStamina() <= MinimumValue)
		{
			UAbilitySystemComponent* AbilitySystemComponent = Data.Target.AbilityActorInfo->AbilitySystemComponent.Get();
			if (AbilitySystemComponent)
			{
				AbilitySystemComponent->RemoveLooseGameplayTag(EQGameplayTag::Character_State_Sprinting);

				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(EQGameplayTag::Character_State_Sprinting);
				AbilitySystemComponent->CancelAbilities(&TagContainer);

				FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromClass(UEQGameplayAbility_Sprint::StaticClass());
				if (Spec)
				{
					Spec->InputPressed = false;
				}
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumValue, GetMaxHealth()));
		SetDamage(MinimumValue);
	}

	if (GetHealth() <= MinimumValue && !bOutOfHealth)
	{
		Data.Target.AddLooseGameplayTag(EQGameplayTag::Character_State_Dead);
		OnOutOfHealth.Broadcast();
	}

	bOutOfHealth = GetHealth() <= MinimumValue;
}

// void UEQCharacterAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
// {
// 	if (Attribute == GetHealthAttribute())
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Health: %f -> %f"), OldValue, NewValue);
// 	}
// }
