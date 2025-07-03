// Fill out your copyright notice in the Description page of Project Settings.

#include "EQWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Animation/EQAnimInstance.h"
#include "Character/EQBaseCharacter.h"
#include "Component/EQCollisionComponent.h"
#include "Component/EQCombatComponent.h"
#include "Data/EQMontageActionData.h"
#include "GameplayTag/EQGameplayTag.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AEQWeapon::AEQWeapon()
{
	CollisionComponent = CreateDefaultSubobject<UEQCollisionComponent>(TEXT("CollisionComponent"));
	CollisionComponent->OnHitDelegate.AddUObject(this, &ThisClass::OnHitCallback);
	CombatType = ECombatType::None;
}

void AEQWeapon::EquipItem()
{
	Super::EquipItem();

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		return;
	}

	CombatComponent = OwnerActor->GetComponentByClass<UEQCombatComponent>();
	if (!CombatComponent)
	{
		return;
	}

	CombatComponent->SetWeapon(this);

	const FName SocketName = CombatComponent->IsCombatEnabled() ? EquipSocketName : UnequipSocketName;
	AttachToOwner(SocketName);

	AEQBaseCharacter* OwnerCharacter = Cast<AEQBaseCharacter>(OwnerActor);
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
	CollisionComponent->AddIgnoredActor(OwnerCharacter);
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

UAnimMontage* AEQWeapon::GetHitReactionMontage(const AActor* Attacker) const
{
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), Attacker->GetActorLocation());
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetOwner()->GetActorRotation(), LookAtRotation);
	const float DeltaZ = DeltaRotation.Yaw;

	EHitDirection HitDirection = EHitDirection::Front;

	if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -45.0f, 45.0f))
	{
		HitDirection = EHitDirection::Front;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 45.0f, 135.0f))
	{
		HitDirection = EHitDirection::Left;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 135.0f, 180.0f)
		|| UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -180.0f, -135.0f))
	{
		HitDirection = EHitDirection::Back;
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -135.0f, -45.0f))
	{
		HitDirection = EHitDirection::Right;
	}

	UAnimMontage* SelectedMontage = nullptr;
	switch (HitDirection)
	{
	case EHitDirection::Front:
		SelectedMontage = GetMontageForTag(EQGameplayTag::Character_Action_HitReaction, 0);
		break;
	case EHitDirection::Back:
		SelectedMontage = GetMontageForTag(EQGameplayTag::Character_Action_HitReaction, 0);
		break;
	case EHitDirection::Left:
		SelectedMontage = GetMontageForTag(EQGameplayTag::Character_Action_HitReaction, 0);
		break;
	case EHitDirection::Right:
		SelectedMontage = GetMontageForTag(EQGameplayTag::Character_Action_HitReaction, 0);
		break;
	}

	return SelectedMontage;
}

void AEQWeapon::OnHitCallback(const FHitResult& HitResult)
{
	AActor* TargetActor = HitResult.GetActor();

	const FVector DamageDirection = GetOwner()->GetActorForwardVector();

	constexpr float AttackDamage = 0.0f;

	//UGameplayStatics::ApplyPointDamage(TargetActor, AttackDamage, DamageDirection, HitResult, GetOwner()->GetInstigatorController(), this, nullptr);
}
