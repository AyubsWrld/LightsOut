// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractorComponent.h"

#include "ActorReferencesUtils.h"

UInteractorComponent::UInteractorComponent()
{
}


void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor*						Owner				{ GetOwner() } ; 
	UInteractionManager*		Interactionmanager	{ GetWorld()->GetSubsystem<UInteractionManager>()} ; 
	
	if ( !Owner || !Interactionmanager )
	{
		UE_LOG(LogTemp, Error, TEXT("[%s]: Failure occured while initializing component: Owner(%d), InteractionManager(%d)"), ANSI_TO_TCHAR(__FUNCTION__), (Owner == nullptr), (Interactionmanager == nullptr));
		
		
	}
	else
	{
		
	}
}

void UInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UInteractorComponent::OnInteractionEvent(FInteractionEvent InteractionEvent)// Subject to change 
{
	
}

[[nodiscard]] FInteractionEvent UInteractorComponent::GetStubEvent()  const
{
	return FInteractionEvent{nullptr, nullptr};
}


