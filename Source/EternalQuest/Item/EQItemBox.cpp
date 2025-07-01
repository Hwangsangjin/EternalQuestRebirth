// Fill out your copyright notice in the Description page of Project Settings.

#include "EQItemBox.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EQPickupItem.h"
#include "Components/BoxComponent.h"
#include "GameplayTag/EQGameplayTag.h"
#include "Physics/EQCollision.h"

AEQItemBox::AEQItemBox()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxComponent"));
	TriggerBoxComponent->SetCollisionProfileName(CPROFILE_EQTRIGGER);
	SetRootComponent(TriggerBoxComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(TriggerBoxComponent);
	SkeletalMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

UAbilitySystemComponent* AEQItemBox::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEQItemBox::PerformInteract(AActor* InteractActor)
{
	if (!SkeletalMeshComponent)
	{
		return;
	}

	SetActorEnableCollision(false);
	SkeletalMeshComponent->Play(false);

	ApplyEffectToTarget(InteractActor);

	UAnimationAsset* AnimAsset = SkeletalMeshComponent->AnimationData.AnimToPlay;
	const UAnimSequence* AnimSequence = Cast<UAnimSequence>(AnimAsset);
	const float AnimDuration = AnimSequence ? AnimSequence->GetPlayLength() : 1.0f;

	for (UEQItemData* ItemData : ItemDataArray)
	{
		if (!ItemData)
		{
			continue;
		}

		FTimerHandle ItemSpawnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(ItemSpawnTimerHandle, [this, InteractActor, ItemData]()
		{
			InvokeGameplayCue(InteractActor);

			AEQPickupItem* PickupItem = GetWorld()->SpawnActorDeferred<AEQPickupItem>(AEQPickupItem::StaticClass(), GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			PickupItem->SetItemData(ItemData);
			PickupItem->FinishSpawning(GetActorTransform());
			PickupItem->Throw();
		}, AnimDuration * 0.75f, false);
	}
}

void AEQItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AEQItemBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	APawn* OverlappingPawn = Cast<APawn>(OtherActor);
	if (!OverlappingPawn || !OverlappingPawn->IsLocallyControlled())
	{
		return;
	}

	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OverlappingPawn);
	if (!TargetAbilitySystemComponent)
	{
		return;
	}

	FGameplayCueParameters Params;
	Params.Instigator = OtherActor;
	TargetAbilitySystemComponent->ExecuteGameplayCue(EQGameplayTag::GameplayCue_InteractPrompt_Show, Params);
}

void AEQItemBox::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	APawn* OverlappingPawn = Cast<APawn>(OtherActor);
	if (!OverlappingPawn || !OverlappingPawn->IsLocallyControlled())
	{
		return;
	}

	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OverlappingPawn);
	if (!TargetAbilitySystemComponent)
	{
		return;
	}

	FGameplayCueParameters Params;
	Params.Instigator = OtherActor;
	TargetAbilitySystemComponent->ExecuteGameplayCue(EQGameplayTag::GameplayCue_InteractPrompt_Hide, Params);
}

void AEQItemBox::ApplyEffectToTarget(AActor* TargetActor)
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetAbilitySystemComponent)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = TargetAbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpecHandle = TargetAbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1,
		EffectContext);
	if (EffectSpecHandle.IsValid())
	{
		TargetAbilitySystemComponent->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
	}
}

void AEQItemBox::InvokeGameplayCue(AActor* TargetActor)
{
	FGameplayCueParameters Parameters;
	Parameters.SourceObject = this;
	Parameters.Instigator = TargetActor;
	Parameters.Location = GetActorLocation();

	AbilitySystemComponent->ExecuteGameplayCue(EQGameplayTag::GameplayCue_ItemBox_Open, Parameters);
}
