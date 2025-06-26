// Fill out your copyright notice in the Description page of Project Settings.

#include "EQWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Animation/EQAnimInstance.h"
#include "Character/EQBaseCharacter.h"
#include "Component/EQCollisionComponent.h"
#include "Component/EQCombatComponent.h"
#include "Data/EQMontageActionData.h"
#include "Kismet/GameplayStatics.h"

AEQWeapon::AEQWeapon()
{
	CollisionComponent = CreateDefaultSubobject<UEQCollisionComponent>(TEXT("CollisionComponent"));
	CollisionComponent->OnHitDelegate.AddUObject(this, &ThisClass::OnHitCallback);
	CombatType = ECombatType::None;
}

void AEQWeapon::EquipItem()
{
	Super::EquipItem();

	CombatComponent = GetOwner()->GetComponentByClass<UEQCombatComponent>();
	if (!CombatComponent)
	{
		return;
	}

	CombatComponent->SetWeapon(this);

	const FName SocketName = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;
	AttachToOwner(SocketName);

	const AEQBaseCharacter* OwnerCharacter = Cast<AEQBaseCharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	UEQAnimInstance* AnimInstance = Cast<UEQAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->UpdateCombatType(CombatType);

	CollisionComponent->SetPrimitiveComponent(SkeletalMeshComponent);
	CollisionComponent->AddIgnoredActor(GetOwner());
}

void AEQWeapon::UnequipItem()
{
	Super::UnequipItem();
}

void AEQWeapon::ActivateCollision()
{
	CollisionComponent->TurnOnCollision();
}

void AEQWeapon::DeactivateCollision()
{
	CollisionComponent->TurnOffCollision();
}

UAnimMontage* AEQWeapon::GetMontageForTag(const FGameplayTag& GameplayTag, const int32 Index) const
{
	return MontageActionData->GetMontageForTag(GameplayTag, Index);
}

UAnimMontage* AEQWeapon::GetRandomMontageForTag(const FGameplayTag& GameplayTag) const
{
	return MontageActionData->GetRandomMontageForTag(GameplayTag);
}

void AEQWeapon::OnHitCallback(const FHitResult& HitResult)
{
	AActor* TargetActor = HitResult.GetActor();

	const FVector DamageDirection = GetOwner()->GetActorForwardVector();

	constexpr float AttackDamage = 0.0f;

	UGameplayStatics::ApplyPointDamage(TargetActor, AttackDamage, DamageDirection, HitResult, GetOwner()->GetInstigatorController(), this, nullptr);
}
