// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionManager.h"

 FInteractionEvent::FInteractionEvent(
	TWeakObjectPtr<UObject> interactable,
	TWeakObjectPtr<UObject> interactor,
	FFloat32				timestamp,
	EInteractionEventTag    interactionEventTag
	) 
		:	Interactable( interactable ) ,
			Interactor(interactor),
			Timestamp(timestamp),
			InteractionEventTag(interactionEventTag)
{}

constexpr EInteractionEventTag operator|(EInteractionEventTag Lhs, EInteractionEventTag Rhs)
{
	using	TUnderlyingType		=		std::underlying_type_t<EInteractionEventTag>;
	return  static_cast<EInteractionEventTag>(static_cast<TUnderlyingType>(Lhs) | static_cast<TUnderlyingType>(Rhs));
}

bool UInteractionManager::ShouldCreateSubsystem(UObject* Outer) const
{
 	//if ( Cast<UWorld>(Outer)->GetNetMode() == NM_Client )
 		//return false; 
	return true; 
}

void UInteractionManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UInteractionManager::Deinitialize()
{
	Super::Deinitialize();
}


void UInteractionManager::BeginDestroy() 
{
	Super::BeginDestroy();
}

void UInteractionManager::PostInitialize() 
{
	Super::PostInitialize();
}

void UInteractionManager::ConsumeEvent(const FInteractionEvent& InteractionEvent) const
{
	using	enum	EInteractionEventTag;
	
	switch (InteractionEvent.GetEventTag())
	{
	case IE_Undefined:
		return ;
		
	case IE_EnvironmentInteraction:
		return ;
		
	case IE_ItemInteraction:
		return ;
		
	case IE_Max:
		return ;
		
	}
	return; 
}
	
