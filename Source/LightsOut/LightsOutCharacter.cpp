// Copyright Epic Games, Inc. All Rights Reserved.

#include "LightsOutCharacter.h"
#include "LightsOutProjectile.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ALightsOutCharacter

ALightsOutCharacter::ALightsOutCharacter()
{
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
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	// Bind reference to global itembroker instance 

	//ItemBroker = MakeShared<IItemBroker>(GetWorld()->GetSubsystem<UIBSingleton>());


}

//////////////////////////////////////////////////////////////////////////// Input

void ALightsOutCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ALightsOutCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALightsOutCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALightsOutCharacter::Look);

		// Interctions
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ALightsOutCharacter::Interact);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ALightsOutCharacter::Move(const FInputActionValue& Value)
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

void ALightsOutCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ALightsOutCharacter::HandleInteractionRequest_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("User Interacted"));
	if (!CameraScanner || !CameraScanner->CurrentActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("CamerScanner doesn't Exist"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s]: Player Location( X: %f, Y: %f, Z: %f)"), ANSI_TO_TCHAR(__FUNCTION__),
		CameraScanner->CurrentActor->GetActorLocation().X,
		CameraScanner->CurrentActor->GetActorLocation().Y,
		CameraScanner->CurrentActor->GetActorLocation().Z
	);

	AActor* Target = CameraScanner->CurrentActor;
	if (Target->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{

		if (IInteractable* I = Cast<IInteractable>(Target)) { I->Interact(GetActorGuid()); }
	}
}

void ALightsOutCharacter::Interact(const FInputActionValue& Value)
{
	HandleInteractionRequest();
}

void ALightsOutCharacter::Tick(float Deltatime)
{
	CameraScanner->Scan();
}

void ALightsOutCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Initialize Camera hit scanner


	ItemBroker = GetWorld()->GetSubsystem<UIBSingleton>();

	PrimaryActorTick.bCanEverTick = true;
	
	CameraScanner = MakeUnique<FCameraHitScanner>(FirstPersonCameraComponent, GetWorld(), this);
}
