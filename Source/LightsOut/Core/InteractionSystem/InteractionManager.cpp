// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionManager.h"

constexpr EInteractionEventTag operator|(EInteractionEventTag Lhs, EInteractionEventTag Rhs)
{
	using	TUnderlyingType		=		std::underlying_type_t<EInteractionEventTag>;
	return  static_cast<EInteractionEventTag>(static_cast<TUnderlyingType>(Lhs) | static_cast<TUnderlyingType>(Rhs));
}

bool UInteractionManager::ShouldCreateSubsystem(UObject* Outer) const
{
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

void UInteractionManager::PostInitialize() override
{
	Super::PostInitialize();
}

bool UInteractionManager::ConsumeEvent(EInteractionEventTag InteractionEvent) const
{
	using	enum	EInteractionEventTag;
	
	switch (InteractionEvent)
	{
		
	case IE_Undefined:
		return true;
		
	case IE_EnvironmentInteraction:
		return true;
		
	case IE_ItemInteraction:
		return true;
		
	case IE_Max:
		return true;
		
	}
	return false; 
}
	
