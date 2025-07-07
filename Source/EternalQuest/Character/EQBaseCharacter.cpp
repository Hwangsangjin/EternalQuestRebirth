// Fill out your copyright notice in the Description page of Project Settings.

#include "EQBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Component/EQAttributeComponent.h"
#include "Component/EQCombatComponent.h"
#include "Component/EQInventoryComponent.h"
#include "Component/EQRotationComponent.h"
#include "Component/EQStateComponent.h"
#include "Component/EQWidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Data/EQCharacterControlData.h"
#include "Engine/DamageEvents.h"
#include "Equipment/EQShield.h"
#include "Equipment/EQWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTag/EQGameplayTag.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Physics/EQCollision.h"
#include "Sound/SoundCue.h"

AEQBaseCharacter::AEQBaseCharacter()
{
	bReplicates = true;
	AbilitySystemComponent = nullptr;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCapsuleComponent()->InitCapsuleSize(34.0f, 88.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_EQCAPSULE);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 450.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	AttributeComponent = CreateDefaultSubobject<UEQAttributeComponent>(TEXT("AttributeComponent"));
	AttributeComponent->OnDeadDelegate.AddUObject(this, &ThisClass::SetDead);

	StateComponent = CreateDefaultSubobject<UEQStateComponent>(TEXT("StateComponent"));

	CombatComponent = CreateDefaultSubobject<UEQCombatComponent>(TEXT("CombatComponent"));

	RotationComponent = CreateDefaultSubobject<UEQRotationComponent>(TEXT("RotationComponent"));

	InventoryComponent = CreateDefaultSubobject<UEQInventoryComponent>(TEXT("InventoryComponent"));

	HealthBarWidgetComponent = CreateDefaultSubobject<UEQWidgetComponent>(TEXT("HealthBarWidgetComponent"));
	HealthBarWidgetComponent->SetupAttachment(RootComponent);
	HealthBarWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidgetComponent->SetDrawSize(FVector2D(200.0f, 20.0f));
}

bool AEQBaseCharacter::IsDead() const
{
	check(StateComponent);

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(EQGameplayTag::Character_State_Dead);

	return StateComponent->IsCurrentStateEqualToAny(TagContainer);
}

void AEQBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CombatComponent->SetCombatEnabled(true);
	AbilitySystemComponent->AddLooseGameplayTag(EQGameplayTag::Character_State_Combat);

	FActorSpawnParameters Parameters;
	Parameters.Owner = this;

	if (WeaponClass)
	{
		AEQWeapon* Weapon = GetWorld()->SpawnActor<AEQWeapon>(WeaponClass, GetActorTransform(), Parameters);
		Weapon->EquipItem();
	}

	if (ShieldClass)
	{
		AEQShield* Shield = GetWorld()->SpawnActor<AEQShield>(ShieldClass, GetActorTransform(), Parameters);
		Shield->EquipItem();
	}
}

UAbilitySystemComponent* AEQBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEQBaseCharacter::SetCharacterControlData(const UEQCharacterControlData* CharacterControlData)
{
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

float AEQBaseCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	check(AttributeComponent);
	check(StateComponent);

	bFacing = UKismetMathLibrary::InRange_FloatFloat(GetDotProductTo(EventInstigator->GetPawn()), -0.1f, 1.0f);

	if (CanPerformBlock(EventInstigator))
	{
		AttributeComponent->TakeDamageAmount(0.0f);
		StateComponent->SetState(EQGameplayTag::Character_State_Blocking);
	}
	else
	{
		AttributeComponent->TakeDamageAmount(ActualDamage);
		StateComponent->SetState(EQGameplayTag::Character_State_Hit);
	}

	StateComponent->ToggleMovementInput(false);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		const FVector ShotDirection = PointDamageEvent->ShotDirection;
		const FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;
		const FVector ImpactDirection = PointDamageEvent->HitInfo.ImpactNormal;
		const FVector HitLocation = PointDamageEvent->HitInfo.Location;

		ImpactEffect(EventInstigator, ImpactPoint);

		HitReaction(EventInstigator->GetPawn());
	}

	return ActualDamage;
}

bool AEQBaseCharacter::IsCombatEnabled() const
{
	check(CombatComponent);
	return CombatComponent->IsCombatEnabled();
}

void AEQBaseCharacter::SetCombatEnabled(bool bEnabled)
{
	check(CombatComponent);
	return CombatComponent->SetCombatEnabled(bEnabled);
}

AEQWeapon* AEQBaseCharacter::GetWeapon() const
{
	check(CombatComponent);
	return CombatComponent->GetWeapon();
}

AEQShield* AEQBaseCharacter::GetShield() const
{
	check(CombatComponent);
	return CombatComponent->GetShield();
}

void AEQBaseCharacter::ActivateWeaponCollision()
{
	check(CombatComponent);
	CombatComponent->GetWeapon()->ActivateCollision();
}

void AEQBaseCharacter::DeactivateWeaponCollision()
{
	check(CombatComponent);
	CombatComponent->GetWeapon()->DeactivateCollision();
}

bool AEQBaseCharacter::CanBlockStance() const
{
	check(StateComponent);
	check(CombatComponent);

	const AEQWeapon* Weapon = CombatComponent->GetWeapon();
	if (!Weapon)
	{
		return false;
	}

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(EQGameplayTag::Character_State_Attacking);
	TagContainer.AddTag(EQGameplayTag::Character_State_Hit);
	TagContainer.AddTag(EQGameplayTag::Character_State_Jumping);

	return StateComponent->IsCurrentStateEqualToAny(TagContainer) == false && Weapon->GetCombatType() == ECombatType::SwordShield;
}

bool AEQBaseCharacter::CanPerformBlock(const AActor* Attacker) const
{
	if (!Attacker)
	{
		return false;
	}

	check(CombatComponent);
	check(AttributeComponent);
	check(StateComponent);

	if (!CombatComponent->IsBlockingEnabled())
	{
		return false;
	}

	FGameplayTagContainer InvalidStates;
	InvalidStates.AddTag(EQGameplayTag::Character_State_Attacking);
	InvalidStates.AddTag(EQGameplayTag::Character_State_Hit);
	InvalidStates.AddTag(EQGameplayTag::Character_State_Jumping);

	if (StateComponent->IsCurrentStateEqualToAny(InvalidStates))
	{
		return false;
	}

	const float Dot = GetDotProductTo(Attacker);
	const bool bIsFacing = UKismetMathLibrary::InRange_FloatFloat(Dot, -0.1f, 1.0f);

	if (!bIsFacing)
	{
		return false;
	}

	return true;
}

bool AEQBaseCharacter::CanPerformParry() const
{
	check(StateComponent);
	check(CombatComponent);
	check(AttributeComponent);

	const AEQWeapon* Weapon = CombatComponent->GetWeapon();
	if (!Weapon)
	{
		return false;
	}

	FGameplayTagContainer GameplayTags;
	GameplayTags.AddTag(EQGameplayTag::Character_State_Attacking);
	GameplayTags.AddTag(EQGameplayTag::Character_State_Rolling);
	GameplayTags.AddTag(EQGameplayTag::Character_State_Hit);
	GameplayTags.AddTag(EQGameplayTag::Character_State_Blocking);
	GameplayTags.AddTag(EQGameplayTag::Character_State_Parrying);
	GameplayTags.AddTag(EQGameplayTag::Character_State_Dead);

	return StateComponent->IsCurrentStateEqualToAny(GameplayTags) == false && Weapon->GetCombatType() == ECombatType::SwordShield;
}

bool AEQBaseCharacter::ParriedAttackSucceed() const
{
	check(StateComponent);

	FGameplayTagContainer GameplayTags;
	GameplayTags.AddTag(EQGameplayTag::Character_State_Parrying);

	return StateComponent->IsCurrentStateEqualToAny(GameplayTags) && bFacing;
}

bool AEQBaseCharacter::CanDrinkPotion() const
{
	check(InventoryComponent);
	check(StateComponent);

	FGameplayTagContainer GameplayTags;
	GameplayTags.AddTag(EQGameplayTag::Character_State_Attacking);
	GameplayTags.AddTag(EQGameplayTag::Character_State_Jumping);
	GameplayTags.AddTag(EQGameplayTag::Character_State_Hit);
	GameplayTags.AddTag(EQGameplayTag::Character_State_Blocking);
	GameplayTags.AddTag(EQGameplayTag::Character_State_Parrying);
	GameplayTags.AddTag(EQGameplayTag::Character_State_Dead);

	return InventoryComponent->GetPotionQuantity() > 0 && StateComponent->IsCurrentStateEqualToAny(GameplayTags) == false;
}

void AEQBaseCharacter::InterruptWhileDrinkingPotion() const
{
	check(StateComponent);

	FGameplayTagContainer GameplayTags;
	GameplayTags.AddTag(EQGameplayTag::Character_State_DrinkingPotion);

	if (StateComponent->IsCurrentStateEqualToAny(GameplayTags))
	{
		if (InventoryComponent)
		{
			InventoryComponent->DespawnPotion();
		}
	}
}

void AEQBaseCharacter::ImpactEffect(const AActor* Attacker, const FVector& Location)
{
	if (CanPerformBlock(Attacker))
	{
		ShieldBlockingEffect(Location);
	}
	else
	{
		if (!ImpactSoundCue || !ImpactNiagaraSystem)
		{
			return;
		}

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSoundCue, Location);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactNiagaraSystem, GetActorLocation(), GetActorRotation());
	}
}

void AEQBaseCharacter::ShieldBlockingEffect(const FVector& Location)
{
	if (!BlockingSoundCue || !BlockingNiagaraSystem)
	{
		return;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), BlockingSoundCue, Location);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BlockingNiagaraSystem, GetActorLocation(), GetActorRotation());
}

void AEQBaseCharacter::HitReaction(const AActor* Attacker)
{
	check(CombatComponent);

	if (CanPerformBlock(Attacker))
	{
		if (UAnimMontage* BlockingHitMontage = CombatComponent->GetWeapon()->GetMontageForTag(EQGameplayTag::Character_Action_BlockingHit))
		{
			PlayAnimMontage(BlockingHitMontage);
		}
	}
	else
	{
		if (UAnimMontage* HitReactionMontage = CombatComponent->GetWeapon()->GetHitReactionMontage(Attacker))
		{
			PlayAnimMontage(HitReactionMontage);
		}
	}
}

void AEQBaseCharacter::SetDead()
{
	StateComponent->SetState(EQGameplayTag::Character_State_Dead);
	GetCharacterMovement()->SetMovementMode(MOVE_None);
	SetActorEnableCollision(false);
	HideHealthBarWidget();
	PlayDeadAnimation();
}

void AEQBaseCharacter::HideHealthBarWidget()
{
	check(HealthBarWidgetComponent);
	HealthBarWidgetComponent->SetHiddenInGame(true);
}

void AEQBaseCharacter::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage);
}
