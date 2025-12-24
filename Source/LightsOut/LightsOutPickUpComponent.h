// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "LightsOutCharacter.h"
#include "LightsOut/Core/InteractionSystem/InteractionManager.h"
#include "LightsOutPickUpComponent.generated.h"

// Declaration of the delegate that will be called when someone picks this up
// The character picking this up is the parameter sent with the notification

// These should just be defined within the InteractionManager 

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, ALightsOutCharacter*, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LIGHTSOUT_API ULightsOutPickUpComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	
	ULightsOutPickUpComponent();
	
	FEventDelegate EventDelegate; 
	
	static FInteractionEvent	GetStubEvent();
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};