// Fill out your copyright notice in the Description page of Project Settings.

#include "EQPickupItem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EQItemData.h"
#include "Components/BoxComponent.h"
#include "Equipment/EQEquipment.h"
#include "GameplayTag/EQGameplayTag.h"
#include "Physics/EQCollision.h"

AEQPickupItem::AEQPickupItem()
{
	PrimaryActorTick.bCanEverTick = true;

	GameplayCueTag = EQGameplayTag::GameplayCue_PickupItem_Spawn;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxComponent"));
	TriggerBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBoxComponent->SetBoxExtent(FVector(64.0f, 64.0f, 64.0f));
	TriggerBoxComponent->SetCollisionObjectType(CCHANNEL_EQPICKUP);
	SetRootComponent(TriggerBoxComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SkeletalMeshComponent->SetupAttachment(RootComponent);
}

UAbilitySystemComponent* AEQPickupItem::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEQPickupItem::SetItemData(UEQItemData* NewItemData)
{
	if (!NewItemData || !SkeletalMeshComponent)
	{
		return;
	}

	ItemData = NewItemData;
	EquipmentClass = ItemData->EquipmentClass;

	const AEQEquipment* EquipmentCDO = EquipmentClass ? EquipmentClass->GetDefaultObject<AEQEquipment>() : nullptr;
	USkeletalMesh* MeshToSet = EquipmentCDO ? EquipmentCDO->GetSkeletalMesh() : nullptr;

	if (!MeshToSet)
	{
		return;
	}

	SkeletalMeshComponent->SetSkeletalMesh(MeshToSet);
}

void AEQPickupItem::Throw()
{
	const FVector StartLocation = GetActorLocation();
	const FVector ForwardDir = GetActorForwardVector();

	// 방향이 너무 작을 경우 대비
	FVector RandomDir = FMath::VRandCone(ForwardDir, FMath::DegreesToRadians(45.0f));
	if (RandomDir.SizeSquared() < KINDA_SMALL_NUMBER)
	{
		RandomDir = ForwardDir;
	}

	const float LaunchDistance = FMath::RandRange(150.0f, 250.0f);
	FVector HorizontalTarget = StartLocation + RandomDir * LaunchDistance;

	// 바닥 Z 계산
	FHitResult Hit;
	const FVector TraceStart = HorizontalTarget + FVector(0.0f, 0.0f, 200.0f);
	const FVector TraceEnd = HorizontalTarget - FVector(0.0f, 0.0f, 1000.0f);
	constexpr float GroundOffset = 5.0f; // 살짝 띄우기 위한 오프셋

	if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility))
	{
		HorizontalTarget.Z = Hit.ImpactPoint.Z + GroundOffset;
	}
	else
	{
		// 실패 시 시작 높이로
		HorizontalTarget.Z = StartLocation.Z + GroundOffset;
	}

	constexpr float MaxHeight = 150.0f;  // 포물선 최대 높이
	constexpr float Duration = 0.4f;
	constexpr float StepTime = 0.01f;
	constexpr int32 TotalSteps = Duration / StepTime;

	struct FLaunchData
	{
		int32 Step = 0;
	};

	TSharedPtr<FLaunchData> LaunchData = MakeShared<FLaunchData>();

	GetWorld()->GetTimerManager().SetTimer(LaunchTimerHandle, [this, StartLocation, HorizontalTarget, MaxHeight, LaunchData, TotalSteps]()
	{
		const float Alpha = static_cast<float>(LaunchData->Step) / static_cast<float>(TotalSteps);

		// 수평 위치 보간
		FVector NewLocation = FMath::Lerp(StartLocation, HorizontalTarget, Alpha);

		// 포물선 높이 계산 (0 ~ MaxHeight ~ 0)
		const float Height = 4.0f * MaxHeight * Alpha * (1.0f - Alpha);  // 0에서 시작해서 중간에 최대, 끝에 0으로 떨어짐

		NewLocation.Z += Height;

		SetActorLocation(NewLocation);

		// 회전
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += 10.0f;
		SetActorRotation(Rotation);

		++LaunchData->Step;
		if (LaunchData->Step >= TotalSteps)
		{
			InvokeGameplayCue();
			GetWorld()->GetTimerManager().ClearTimer(LaunchTimerHandle);
		}
	}, StepTime, true);

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 2.0f);
	DrawDebugSphere(GetWorld(), HorizontalTarget, 10.0f, 12, FColor::Green, false, 2.0f);
}

void AEQPickupItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

void AEQPickupItem::NotifyActorBeginOverlap(AActor* OtherActor)
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

void AEQPickupItem::NotifyActorEndOverlap(AActor* OtherActor)
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

void AEQPickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!EquipmentClass)
	{
		return;
	}

	const AEQEquipment* EquipmentCDO = EquipmentClass ? EquipmentClass->GetDefaultObject<AEQEquipment>() : nullptr;
	USkeletalMesh* MeshToSet = EquipmentCDO ? EquipmentCDO->GetSkeletalMesh() : nullptr;

	if (!MeshToSet)
	{
		return;
	}

	SkeletalMeshComponent->SetSkeletalMesh(MeshToSet);
	SkeletalMeshComponent->SetSimulatePhysics(true);
}

void AEQPickupItem::BeginPlay()
{
	Super::BeginPlay();
}

void AEQPickupItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += 10.f * DeltaSeconds;
	SetActorRotation(Rotation);
}

void AEQPickupItem::InvokeGameplayCue()
{
	if (!AbilitySystemComponent || !GameplayCueTag.IsValid())
	{
		return;
	}

	FGameplayCueParameters Parameters;
	Parameters.SourceObject = this;
	Parameters.Instigator = GetOwner();
	Parameters.Location = GetActorLocation();

	AbilitySystemComponent->ExecuteGameplayCue(GameplayCueTag, Parameters);
}

void AEQPickupItem::PerformInteract(AActor* InteractActor)
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = InteractActor;

	AEQEquipment* SpawnItem = GetWorld()->SpawnActor<AEQEquipment>(EquipmentClass, GetActorTransform(), SpawnParameters);
	if (!SpawnItem)
	{
		return;
	}

	SpawnItem->EquipItem();
	Destroy();
}
