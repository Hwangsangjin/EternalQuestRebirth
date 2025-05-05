// Fill out your copyright notice in the Description page of Project Settings.

#include "EQCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Component/EQAttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AEQCharacter::AEQCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeRotation(FRotator(-30.0f, 0.0f, 0.0f));
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	AttributeComponent = CreateDefaultSubobject<UEQAttributeComponent>(TEXT("AttributeComponent"));

	MovingThreshold = 3.0f;
	StaminaCost = 0.1f;
	SprintCost = 1.0f;
	SprintSpeed = 750.0f;
	NormalSpeed = 500.0f;
}

void AEQCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller is nullptr."));
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (Subsystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enhanced Input Local Player Subsystem is nullptr."));
		return;
	}

	Subsystem->AddMappingContext(DefaultMappingContext, 0);
}

void AEQCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enhanced Input Component is nullptr."));
		return;
	}

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::Sprinting);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::StopSprint);
}

void AEQCharacter::Move(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is nullptr."));
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

void AEQCharacter::Look(const FInputActionValue& Value)
{
	if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Controller is nullptr."));
		return;
	}

	const FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

bool AEQCharacter::IsMoving() const
{
	const UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (Movement == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Movement Component is nullptr."));
		return false;
	}

	return Movement->Velocity.Size2D() > MovingThreshold && Movement->GetCurrentAcceleration() != FVector::Zero();
}

void AEQCharacter::Sprinting()
{
	if (AttributeComponent->CheckHasEnoughStamina(SprintCost) && IsMoving())
	{
		AttributeComponent->ToggleRegainStamina(false);
		AttributeComponent->DecreaseStamina(StaminaCost);

		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
	else
	{
		StopSprint();
	}
}

void AEQCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	AttributeComponent->ToggleRegainStamina(true);
}
