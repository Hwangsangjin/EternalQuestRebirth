// Fill out your copyright notice in the Description page of Project Settings.

#include "EQNonPlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Attribute/EQCharacterAttributeSet.h"
#include "Component/EQAttributeComponent.h"
#include "Component/EQCombatComponent.h"
#include "Component/EQStateComponent.h"
#include "Component/EQWidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Equipment/EQWeapon.h"
#include "GameplayTag/EQGameplayTag.h"
#include "Item/EQItemBox.h"
#include "Kismet/GameplayStatics.h"
#include "Physics/EQCollision.h"

AEQNonPlayerCharacter::AEQNonPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UEQCharacterAttributeSet>(TEXT("AttributeSet"));

	TargetingSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TargetingSphereComponent"));
	TargetingSphereComponent->SetupAttachment(RootComponent);
	TargetingSphereComponent->SetCollisionObjectType(CCHANNEL_EQTARGET);
	TargetingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetingSphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("LockOnWidgetComponent"));
	LockOnWidgetComponent->SetupAttachment(RootComponent);
	LockOnWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	LockOnWidgetComponent->SetVisibility(false);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AEQNonPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystem();
	BindAttributeEvent();
}

void AEQNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilitySystem();
}

void AEQNonPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!HealthBarWidgetComponent)
	{
		return;
	}

	const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!PlayerController)
	{
		return;
	}

	// 플레이어 시점 위치
	FVector CameraLocation;
	FRotator CameraRotation;
	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// 타겟 위치
	const FVector TargetLocation = GetActorLocation() + FVector(0.0f, 0.0f, 80.f);

	// 거리 검사
	constexpr float MaxUIDistance = 3000.0f; // UI를 숨길 최대 거리 (예: 3000cm = 30m)
	const float Distance = FVector::Dist(CameraLocation, TargetLocation);
	if (Distance > MaxUIDistance)
	{
		if (HealthBarWidgetComponent->IsVisible())
		{
			HealthBarWidgetComponent->SetVisibility(false);
		}

		return;
	}

	// 라인트레이스로 시야 검사
	FHitResult HitResult;
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TargetLocation, ECC_Visibility);
	const bool bShouldBeVisible = !bHit || HitResult.GetActor() == this; // 라인트레이스가 안 닿았거나, 닿은 게 자기 자신이면 보이게

	// 가시성 결정
	if (HealthBarWidgetComponent->IsVisible() != bShouldBeVisible)
	{
		HealthBarWidgetComponent->SetVisibility(bShouldBeVisible);
	}

	// 디버그 시각화
	const FColor Color = bShouldBeVisible ? FColor::Green : FColor::Red;
	//DrawDebugLine(GetWorld(), CameraLocation, TargetLocation, Color, false, 0.1f, 0, 1.0f);
}

void AEQNonPlayerCharacter::PerformAttack(const FGameplayTag& AttackTypeTag, FOnMontageEnded& OnMontageEnded)
{
	check(StateComponent);
	check(CombatComponent);
	check(AttributeComponent);

	const AEQWeapon* Weapon = CombatComponent->GetWeapon();
	if (!Weapon)
	{
		return;
	}

	StateComponent->SetState(EQGameplayTag::Character_State_Attacking);
	//CombatComponent->SetLastAttack(AttackTypeTag);
	AttributeComponent->ToggleRegainStamina(false);

	UAnimMontage* Montage = Weapon->GetRandomMontageForTag(AttackTypeTag);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!Montage || !AnimInstance)
	{
		return;
	}

	AnimInstance->Montage_Play((Montage));
	AnimInstance->Montage_SetEndDelegate(OnMontageEnded, Montage);

	//const float StaminaCost = Weapon->GetStaminaCost(AttackMontage);
	//AttributeComponent->DecreaseStamina(StaminaCost);
	AttributeComponent->ToggleRegainStamina(true);
}

void AEQNonPlayerCharacter::Parried()
{
	Super::Parried();
}

void AEQNonPlayerCharacter::InitAbilitySystem()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InitStatEffectClass, 1.0f, EffectContextHandle);
	if (EffectSpecHandle.IsValid())
	{
		AbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}

	if (AbilitySystemComponent)
	{
		for (TSubclassOf Ability : StartAbilities)
		{
			if (Ability)
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Ability, 1, 0, this));
			}
		}
	}
}

void AEQNonPlayerCharacter::BindAttributeEvent()
{
	const UEQCharacterAttributeSet* CurrentAttributeSet = AbilitySystemComponent->GetSet<UEQCharacterAttributeSet>();
	if (!CurrentAttributeSet)
	{
		return;
	}

	CurrentAttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::SetDead);
}

FOnTargetOutOfHealthDelegate& AEQNonPlayerCharacter::OnTargetOutOfHealth()
{
	return OnTargetOutOfHealthDelegate;
}

void AEQNonPlayerCharacter::SetDead()
{
	Super::SetDead();

	OnTargetOutOfHealthDelegate.Broadcast();
	LockOnWidgetComponent->SetHiddenInGame(true);

	const AAIController* AIController = Cast<AAIController>(GetController());
	if (!AIController)
	{
		return;
	}

	AIController->GetBrainComponent()->StopLogic(TEXT("Dead"));

	const FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 150.0f;
	const FRotator SpawnRotation = GetActorRotation();
	const FTransform SpawnTransform = FTransform(SpawnRotation, SpawnLocation);

	AEQItemBox* ItemBox = GetWorld()->SpawnActorDeferred<AEQItemBox>(ItemBoxClass, SpawnTransform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!ItemBox)
	{
		return;
	}

	UGameplayStatics::FinishSpawningActor(ItemBox, SpawnTransform);
}

void AEQNonPlayerCharacter::OnTargeted(bool bTargeted)
{
	check(LockOnWidgetComponent);
	LockOnWidgetComponent->SetVisibility(bTargeted);
}

bool AEQNonPlayerCharacter::CanBeTargeted()
{
	check(StateComponent);

	FGameplayTagContainer TagCheck;
	TagCheck.AddTag(EQGameplayTag::Character_State_Dead);
	return StateComponent->IsCurrentStateEqualToAny(TagCheck) == false;
}
