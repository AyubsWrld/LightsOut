// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Camera/CameraComponent.h"
#include "Generics/Interactable.h"
#include "Generics/ItemBroker.h"
#include "Core/IBSingleton.h"
#include "Templates/SharedPointer.h"
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

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;


	TUniquePtr<FCameraHitScanner> CameraScanner; 

	IItemBroker* ItemBroker; 
	
public:
	ALightsOutCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float Deltatime) override;

protected:

	void Interact(const FInputActionValue& Value);

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

