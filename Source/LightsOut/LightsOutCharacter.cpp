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

		// Equip Items 

		EnhancedInputComponent->BindAction(EquipSlotZeroAction, ETriggerEvent::Triggered, this, &ALightsOutCharacter::EquipSlot0);
		EnhancedInputComponent->BindAction(EquipSlotOneAction, ETriggerEvent::Triggered, this, &ALightsOutCharacter::EquipSlot1);
		EnhancedInputComponent->BindAction(EquipSlotTwoAction, ETriggerEvent::Triggered, this, &ALightsOutCharacter::EquipSlot2);
		EnhancedInputComponent->BindAction(EquipSlotThreeAction, ETriggerEvent::Triggered, this, &ALightsOutCharacter::EquipSlot3);

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

void ALightsOutCharacter::ServerHandleInteractionRequest_Implementation()
{
	FHitResult HitResult; 
	FCollisionQueryParams Parameters; 

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	FRotator Rotation = PlayerController->GetControlRotation().GetNormalized();
	Parameters.AddIgnoredActor(this);

	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector End = Start + Rotation.Vector() * 300.0f; 

	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Pawn, Parameters);

	AActor* HitActor = HitResult.GetActor();
	if (HitResult.bBlockingHit && IsValid(HitActor) && HitActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		/* This is atomic and should be wrapped into a single call */
		IInteractable* Interactable = Cast<IInteractable>(HitActor);
		Interactable->Interact(GetActorGuid()); // Perhaps we can deduce actor guid elsewhere? or maybe global delegate the interaction. 
		//ClientUpdateHUD(Cast<AItemBase>(Interactable));
	}
}

void ALightsOutCharacter::ServerHandleEquipRequest_Implementation(AItemBase* Item)
{
	if (!Item)
		return;
	if (ItemBroker->PlayerOwnsItem(*Item, GetActorGuid())) // Just make this a pointer
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]: Player can equip item %s"), ANSI_TO_TCHAR(__FUNCTION__), *Item->GetActorGuid().ToString());
	}
}


void ALightsOutCharacter::Interact(const FInputActionValue& Value)
{
	FRotator Rotation = FirstPersonCameraComponent->GetComponentRotation();
	UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), Rotation.Roll, Rotation.Pitch, Rotation.Yaw);
	ServerHandleInteractionRequest();
}

void ALightsOutCharacter::EquipSlot0(const FInputActionValue& Value)
{
	/* ... Server Validation RPC ... */
	check(PlayerHUD);
	if (!PlayerHUD || !PlayerInventory )
		return; 
	Cast<ULightsOutCharacterHUD>(PlayerHUD)->DebugItemSlot(0);
	if (ItemBroker)
	{
		ServerHandleEquipRequest(PlayerInventory->GetItemAtIndex(0));
	}
}

void ALightsOutCharacter::EquipSlot1(const FInputActionValue& Value)
{
	/* ... Server Validation RPC ... */
	check(PlayerHUD);
	if (!PlayerHUD)
		return; 
	Cast<ULightsOutCharacterHUD>(PlayerHUD)->DebugItemSlot(1);
}

void ALightsOutCharacter::EquipSlot2(const FInputActionValue& Value)
{
	/* ... Server Validation RPC ... */

	check(PlayerHUD);
	if (!PlayerHUD)
		return; 
	Cast<ULightsOutCharacterHUD>(PlayerHUD)->DebugItemSlot(2);
}

void ALightsOutCharacter::EquipSlot3(const FInputActionValue& Value)
{
	/* ... Server Validation RPC ... */
	check(PlayerHUD);
	if (!PlayerHUD)
		return; 
	Cast<ULightsOutCharacterHUD>(PlayerHUD)->DebugItemSlot(3);
}

void ALightsOutCharacter::ClientUpdateInventory_Implementation(AItemBase* Item)
{
	if (!PlayerInventory || !Item)
		return;
	PlayerInventory->AddItem(Item);
}
void ALightsOutCharacter::ClientUpdateHUD_Implementation(AItemBase* Item)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Updating Client HUD"), ANSI_TO_TCHAR(__FUNCTION__));
	if (!PlayerHUD || !Item)
		return;
	ULightsOutCharacterHUD* HUD = Cast<ULightsOutCharacterHUD>(PlayerHUD); 
	HUD->UpdateHUD(Item);
}

void ALightsOutCharacter::Tick(float Deltatime)
{
	/* 
		Why does having this within IsLocallyControlled cause compilation errors?,
		Actor ticks are called on the server as well, this is nullptr. unless locally relevant. 
	*/
	//CameraScanner->Scan();
}

void ALightsOutCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Initialize Camera hit scanner


	ItemBroker = GetWorld()->GetSubsystem<UIBSingleton>();
	PrimaryActorTick.bCanEverTick = true;
	CameraScanner = MakeUnique<FCameraHitScanner>(FirstPersonCameraComponent, GetWorld(), this);

	/* Why? Explain this */
	if (IsLocallyControlled() && Controller)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Controller);
		PlayerHUD = CreateWidget<ULightsOutCharacterHUD>(PlayerController, PlayerHUDClassReference);
		if (PlayerHUD)
		{
			PlayerHUD->AddToPlayerScreen();
			auto tmp = Cast<ULightsOutCharacterHUD>(PlayerHUD);
			if (tmp)
				tmp->SetItemSlot();
			return; 
		}
		UE_LOG(LogTemp, Error, TEXT("Undefined HUD"));
	}
}

