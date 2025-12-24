// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractorComponent.h"

#include "ActorReferencesUtils.h"

UInteractorComponent::UInteractorComponent()
{
}


void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();
	if ( AActor* Owner = GetOwner() ; Owner )
	{
		UE_LOG(LogTemp, Warning, TEXT("Found Owner"));
	}
}


void UInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

FInteractionEvent UInteractorComponent::GetStubEvent()  const
{
	return FInteractionEvent{nullptr, nullptr};
}
