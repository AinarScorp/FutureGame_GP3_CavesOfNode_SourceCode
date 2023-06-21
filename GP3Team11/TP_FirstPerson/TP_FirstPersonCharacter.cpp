// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_FirstPersonCharacter.h"
#include "TP_FirstPersonProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TP_FirstPersonGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


//////////////////////////////////////////////////////////////////////////
// ATP_FirstPersonCharacter

ATP_FirstPersonCharacter::ATP_FirstPersonCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	// PlayerWeaponManager = CreateDefaultSubobject<UWeaponManager>(TEXT("WeaponManagerComponent"));
	WeaponSwitching = CreateDefaultSubobject<UWeaponSwitcher>(TEXT("WeaponSwitching"));
}

void ATP_FirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	settings = UGameSettings::GetGameSettings();
	// if(!PlayerWeaponManager)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("PlayerWeaponManager is null"));
	// }
	//PlayerWeaponManager = Cast<ATP_FirstPersonGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->PlayerWeaponManager;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void ATP_FirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATP_FirstPersonCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATP_FirstPersonCharacter::Look);

		//Sprinting
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ATP_FirstPersonCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATP_FirstPersonCharacter::Unsprint);
	}
}


void ATP_FirstPersonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ATP_FirstPersonCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	LookAxisVector *= settings->Sensitivity;
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATP_FirstPersonCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}
//
// bool ATP_FirstPersonCharacter::GetHasRifle()
// {
// 	return bHasRifle;
// }

void ATP_FirstPersonCharacter::Sprint(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ATP_FirstPersonCharacter::Unsprint(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}