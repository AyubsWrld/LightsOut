// Copyright Epic Games, Inc. All Rights Reserved.

#include "LightsOutPickUpComponent.h"

ULightsOutPickUpComponent::ULightsOutPickUpComponent()
{
	SphereRadius = 32.f;
}

void ULightsOutPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	if ( UInteractionManager*  InteractionManager = GetWorld()->GetSubsystem<UInteractionManager>() ;  InteractionManager ) 
	{
		UE_LOG(LogTemp, Warning, TEXT(""));
	}
		
}

void ULightsOutPickUpComponent::OnSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	)
{
	return;
}

/* Calls move ctor  */ 
FInteractionEvent	ULightsOutPickUpComponent::GetStubEvent()  
{
	return FInteractionEvent{nullptr, nullptr};
}
