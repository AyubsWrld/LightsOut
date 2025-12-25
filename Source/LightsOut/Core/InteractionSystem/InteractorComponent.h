// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LightsOut/Core/InteractionSystem/InteractionManager.h"
#include "Camera/CameraComponent.h"
#include "InteractorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIGHTSOUT_API UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

	const ULONG64						InteractionRange	{500};
	
	UCameraComponent*					OwnerCamera;			
	
	UInteractionManager*				InteractionManager;
	
	FEventDelegate						InteractionDelegate;
	
	AActor*								Owner;
	
public:	
	
	UInteractorComponent();
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnInteract();
	
	void BindInteractionManager(); 
	
	void BindOwnerCameraComponent(UCameraComponent* CameraComponent) { this->OwnerCamera = CameraComponent; }
	
	UObject* GetObjectOfInterest() const; 
	
	FInteractionEvent GetStubEvent() const ;
		
};
