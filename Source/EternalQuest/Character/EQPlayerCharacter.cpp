// Fill out your copyright notice in the Description page of Project Settings.

#include "EQPlayerCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/EQAnimInstance.h"
#include "Attribute/EQCharacterAttributeSet.h"
#include "Camera/CameraComponent.h"
#include "Component/EQAttributeComponent.h"
#include "Component/EQCombatComponent.h"
#include "Component/EQStateComponent.h"
#include "Component/EQTargetingComponent.h"
#include "Component/EQWidgetComponent.h"
#include "Data/EQCharacterControlData.h"
#include "Equipment/EQWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/EQPlayerState.h"

AEQPlayerCharacter::AEQPlayerCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	TargetingComponent = CreateDefaultSubobject<UEQTargetingComponent>(TEXT("TargetingComponent"));

	HealthBarWidgetComponent->SetVisibility(false);

	CharacterControlType = ECharacterControlType::Default;
}

void AEQPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilitySystem();
	BindAttributeEvent();
	SetupInputComponent();
}

void AEQPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilitySystem();
	BindAttributeEvent();
	SetupInputComponent();
}

void AEQPlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (!IsLocallyControlled())
	{
		return;
	}

	SetCharacterControl(CharacterControlType);
}

void AEQPlayerCharacter::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	check(CharacterControlMap[NewCharacterControlType]);

	const UEQCharacterControlData* NewCharacterControl = CharacterControlMap[NewCharacterControlType];
	if (!NewCharacterControl)
	{
		return;
	}

	SetCharacterControlData(NewCharacterControl);

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!Subsystem)
	{
		return;
	}

	Subsystem->ClearAllMappings();

	const UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
	if (!NewMappingContext)
	{
		return;
	}

	Subsystem->AddMappingContext(NewMappingContext, 0);
}

void AEQPlayerCharacter::SetCharacterControlData(const UEQCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	check(SpringArmComponent);

	SpringArmComponent->TargetArmLength = CharacterControlData->TargetArmLength;
	SpringArmComponent->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	SpringArmComponent->bInheritPitch = CharacterControlData->bInheritPitch;
	SpringArmComponent->bInheritYaw = CharacterControlData->bInheritYaw;
	SpringArmComponent->bInheritRoll = CharacterControlData->bInheritRoll;
	SpringArmComponent->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
	SpringArmComponent->SetRelativeRotation(CharacterControlData->RelativeRotation);
}

void AEQPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupInputComponent();
}

void AEQPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		FGameplayAbilitySpec StaminaRegenAbilitySpec(StaminaRegenAbilityClass);
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StaminaRegenAbilitySpec));
		AbilitySystemComponent->TryActivateAbilityByClass(StaminaRegenAbilityClass);

		APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
		//PlayerController->ConsoleCommand(TEXT("ShowDebug AbilitySystem"));
	}
}

void AEQPlayerCharacter::InitAbilitySystem()
{
	if (AbilitySystemComponent)
	{
		return;
	}

	AEQPlayerState* EQPlayerState = GetPlayerState<AEQPlayerState>();
	if (!EQPlayerState)
	{
		return;
	}

	AbilitySystemComponent = EQPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(EQPlayerState, this);

	if (HasAuthority())
	{
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			AbilitySystemComponent->GiveAbility(StartSpec);
		}

		for (const auto& StartInputAbility : StartInputAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			StartSpec.InputID = StartInputAbility.Key;
			AbilitySystemComponent->GiveAbility(StartSpec);
		}
	}
}

void AEQPlayerCharacter::BindAttributeEvent()
{
	const UEQCharacterAttributeSet* CurrentAttributeSet = AbilitySystemComponent->GetSet<UEQCharacterAttributeSet>();
	if (!CurrentAttributeSet)
	{
		return;
	}

	CurrentAttributeSet->OnOutOfHealth.AddDynamic(this, &ThisClass::SetDead);
}

void AEQPlayerCharacter::SetupInputComponent()
{
	if (!InputComponent)
	{
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::InputPressed, EAbilityType::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::InputReleased, EAbilityType::Jump);
	EnhancedInputComponent->BindAction(SprintAndRollAction, ETriggerEvent::Canceled, this, &ThisClass::InputPressed, EAbilityType::Roll);
	EnhancedInputComponent->BindAction(SprintAndRollAction, ETriggerEvent::Canceled, this, &ThisClass::InputPressed, EAbilityType::Roll);
	EnhancedInputComponent->BindAction(SprintAndRollAction, ETriggerEvent::Triggered, this, &ThisClass::InputPressed, EAbilityType::Sprint);
	EnhancedInputComponent->BindAction(SprintAndRollAction, ETriggerEvent::Completed, this, &ThisClass::InputReleased, EAbilityType::Sprint);
	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::InputPressed, EAbilityType::Interact);
	EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &ThisClass::InputPressed, EAbilityType::ToggleCombat);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ThisClass::InputPressed, EAbilityType::Attack);
	EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Ongoing, this, &ThisClass::InputPressed, EAbilityType::Skill);
	EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Completed, this, &ThisClass::InputReleased, EAbilityType::Skill);
	EnhancedInputComponent->BindAction(LockOnTargetAction, ETriggerEvent::Started, this, &ThisClass::LockOnTarget);
	EnhancedInputComponent->BindAction(LeftTargetAction, ETriggerEvent::Started, this, &ThisClass::LeftTarget);
	EnhancedInputComponent->BindAction(RightTargetAction, ETriggerEvent::Started, this, &ThisClass::RightTarget);
	EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Started, this, &ThisClass::Blocking);
	EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ThisClass::BlockingEnd);
	EnhancedInputComponent->BindAction(ParryAction, ETriggerEvent::Started, this, &ThisClass::Parrying);
	EnhancedInputComponent->BindAction(ConsumeAction, ETriggerEvent::Started, this, &ThisClass::Consume);
}

void AEQPlayerCharacter::InputPressed(EAbilityType AbilityType)
{
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromInputID(static_cast<int32>(AbilityType));
	if (!Spec)
	{
		return;
	}

	Spec->InputPressed = true;

	if (Spec->IsActive())
	{
		AbilitySystemComponent->AbilitySpecInputPressed(*Spec);
	}
	else
	{
		AbilitySystemComponent->TryActivateAbility(Spec->Handle);
	}
}

void AEQPlayerCharacter::InputReleased(EAbilityType AbilityType)
{
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromInputID(static_cast<int32>(AbilityType));
	if (!Spec)
	{
		return;
	}

	Spec->InputPressed = false;

	if (Spec->IsActive())
	{
		AbilitySystemComponent->AbilitySpecInputReleased(*Spec);
	}
}

void AEQPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (AttackMontage && GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage))
	{
		return;
	}

	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotator(0.0f, Rotation.Yaw, 0.0f);

	const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector, MovementVector.Y);
	AddMovementInput(RightVector, MovementVector.X);
}

void AEQPlayerCharacter::Look(const FInputActionValue& Value)
{
	if (TargetingComponent && TargetingComponent->IsLockOn())
	{
		return;
	}

	const FVector2D LookVector = Value.Get<FVector2D>();
	constexpr float LookSensitivity = 1.0f;

	AddControllerYawInput(LookVector.X * LookSensitivity);
	AddControllerPitchInput(LookVector.Y * LookSensitivity);
}

void AEQPlayerCharacter::LockOnTarget()
{
	check(TargetingComponent);
	TargetingComponent->ToggleLockOn();
}

void AEQPlayerCharacter::LeftTarget()
{
	check(TargetingComponent);
	TargetingComponent->SwitchingLockedOnActor(ESwitchingDirection::Left);
}

void AEQPlayerCharacter::RightTarget()
{
	check(TargetingComponent);
	TargetingComponent->SwitchingLockedOnActor(ESwitchingDirection::Right);
}

void AEQPlayerCharacter::Blocking()
{
	check(StateComponent);
	check(CombatComponent);

	const AEQWeapon* Weapon = CombatComponent->GetWeapon();
	if (!Weapon)
	{
		return;
	}

	if (!CanBlockStance())
	{
		return;
	}

	UEQAnimInstance* AnimInstance = Cast<UEQAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->UpdateBlocking(true);

	constexpr float BlockingSpeed = 250.0f;
	GetCharacterMovement()->MaxWalkSpeed = BlockingSpeed;

	AbilitySystemComponent->AddLooseGameplayTag(EQGameplayTag::Character_State_Blocking);
	StateComponent->SetState(EQGameplayTag::Character_State_Blocking);
	CombatComponent->SetBlockingEnabled(true);
}

void AEQPlayerCharacter::BlockingEnd()
{
	check(StateComponent);
	check(CombatComponent);

	UEQAnimInstance* AnimInstance = Cast<UEQAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance)
	{
		return;
	}

	AnimInstance->UpdateBlocking(false);

	AbilitySystemComponent->RemoveLooseGameplayTag(EQGameplayTag::Character_State_Blocking);
	StateComponent->ClearState();
	CombatComponent->SetBlockingEnabled(false);

	constexpr float NormalSpeed = 500.0f;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void AEQPlayerCharacter::Parrying()
{
	check(CombatComponent);
	check(StateComponent);
	check(AttributeComponent);

	if (CanPerformParry())
	{
		if (const AEQWeapon* Weapon = CombatComponent->GetWeapon())
		{
			UAnimMontage* ParryingMontage = Weapon->GetMontageForTag(EQGameplayTag::Character_State_Parrying);

			StateComponent->ToggleMovementInput(false);
			AttributeComponent->ToggleRegainStamina(false);
			AttributeComponent->DecreaseStamina(10.0f);

			PlayAnimMontage(ParryingMontage);

			AttributeComponent->ToggleRegainStamina(true);
		}
	}
}

void AEQPlayerCharacter::Consume()
{
	check(StateComponent);
	check(AbilitySystemComponent);

	if (CanDrinkPotion())
	{
		StateComponent->SetState(EQGameplayTag::Character_State_DrinkingPotion);

		if (!DrinkMontage)
		{
			return;
		}

		PlayAnimMontage(DrinkMontage);
	}

	const FGameplayEffectSpecHandle PotionEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(PotionHealEffect, 1.0f, AbilitySystemComponent->MakeEffectContext());

	if (PotionEffectSpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PotionEffectSpecHandle.Data.Get());
	}
}
