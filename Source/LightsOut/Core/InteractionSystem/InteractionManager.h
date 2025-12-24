// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "InteractionManager.generated.h"

/**
 * 
 */

/* Move these into their own struct Header */ 


enum class EInteractionEventTag
{
	IE_Undefined,
	IE_EnvironmentInteraction,
	IE_ItemInteraction,
	IE_Max,
};

/* How does this interact when the move constructor is called? */ 
/* Figure out the copy */ 

struct FInteractionEvent
{
	TWeakObjectPtr<UObject>		Interactable{}; 
	TWeakObjectPtr<UObject>		Interactor{}; 
	FFloat32					Timestamp{}; 
	EInteractionEventTag		InteractionEventTag{};
	
	
	FInteractionEvent(
		TWeakObjectPtr<UObject> interactable,
		TWeakObjectPtr<UObject> interactor,
		FFloat32				timestamp = -1,
		EInteractionEventTag    interactionEventTag = EInteractionEventTag::IE_Undefined
		);
	
	EInteractionEventTag	GetEventTag() const { return InteractionEventTag; }
	
};

/* void(EInteractionEventTag);) */ 

DECLARE_MULTICAST_DELEGATE_OneParam(FEventDelegate, EInteractionEventTag);
DECLARE_MULTICAST_DELEGATE_OneParam(FItemInteraction, EInteractionEventTag);

UCLASS()
class LIGHTSOUT_API UInteractionManager : public UWorldSubsystem
{
	
	GENERATED_BODY()
	
	FEventDelegate InteractionEventDelegate; 
	
public: 

	
	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	virtual void BeginDestroy() override;
	
	virtual void PostInitialize() override;
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	/* Handler */ 
	void ConsumeEvent(const FInteractionEvent& InteractionEvent) const;
	
	void BindEvent(FEventDelegate Delegate);
	
	FEventDelegate& GetEventDelegateHandle() { return InteractionEventDelegate; }
	
};
