// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Camera/CameraComponent.h"
#include "Generics/Interactable.h"
#include "Generics/ItemBroker.h"
#include "Core/IBSingleton.h"
#include "LightsOut/UI/HUD/LightsOutCharacterHUD.h"
#include "LightsOut/PlayerComponents/Inventory.h"
#include "LightsOutCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

struct FCameraHitScanner
{
	FHitResult            HitResult; 
	FVector               Start;
	FVector               End;
	FCollisionQueryParams QueryParams;

	UCameraComponent*     Camera       = nullptr ; 
	UWorld* World                      = nullptr ; 
	AActor*               CurrentActor = nullptr; 


	FCameraHitScanner(UCameraComponent* camera, UWorld* world, AActor* ignored)
	{
		Camera = camera;
		World  = world ;
		QueryParams.AddIgnoredActor(ignored);
		UE_LOG(LogTemp, Warning, TEXT("Initialized CameraHitScanner"));
	}

	void Scan()
	{
		if (World == nullptr || Camera == nullptr)
			return;  // Exception 

		Start = Camera->GetComponentLocation(); 
		End = Start + Camera->GetForwardVector() * 1000.0f; 
		World->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Pawn, QueryParams);
		
		AActor* tmpActor = HitResult.GetActor(); 
		if (HitResult.bBlockingHit && IsValid(tmpActor) && tmpActor != CurrentActor)
		{
			CurrentActor = tmpActor; 
		}
	}

	UCameraComponent* GetCamera()
	{
		return Camera;
	}
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ALightsOutCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category=UI);
	UUserWidget* PlayerHUD;

	UPROPERTY(EditAnywhere, Category=UI)
	TSubclassOf<UUserWidget> PlayerHUDClassReference; 

	UPROPERTY(EditAnywhere, Category=Inventory)
	UInventory* PlayerInventory{};

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EquipSlotZeroAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EquipSlotOneAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EquipSlotTwoAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EquipSlotThreeAction;


	TUniquePtr<FCameraHitScanner> CameraScanner; 

	IItemBroker* ItemBroker; 
	
public:
	ALightsOutCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float Deltatime) override;

protected:

	UFUNCTION(Server, Reliable)
	void ServerHandleInteractionRequest(); 


	UFUNCTION(Client, Reliable)
	void ClientUpdateHUD(AItemBase* Item); 

	UFUNCTION(Client, Reliable)
	void ClientUpdateInventory(AItemBase* Item);

	UFUNCTION(Server, Reliable)
	void ServerHandleEquipRequest(AItemBase* Item);
	void Interact(const FInputActionValue& Value);

	void EquipSlot0(const FInputActionValue& Value);
	void EquipSlot1(const FInputActionValue& Value);
	void EquipSlot2(const FInputActionValue& Value);
	void EquipSlot3(const FInputActionValue& Value);

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);


protected:
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

