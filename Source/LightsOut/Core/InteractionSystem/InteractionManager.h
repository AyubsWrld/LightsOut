// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "InteractionManager.generated.h"

/**
 * 
 */

enum class EInteractionEventTag
{
	IE_Undefined,
	IE_EnvironmentInteraction,
	IE_ItemInteraction,
	IE_Max,
};

UCLASS()
class LIGHTSOUT_API UInteractionManager : public UWorldSubsystem
{
	
	GENERATED_BODY()
	
public: 
	
	// USubsystem implementation Begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;
	
	virtual void BeginDestroy() override;
	
	virtual void PostInitialize() override;
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	
	bool ConsumeEvent(EInteractionEventTag InteractionEvent) const;
	// USubsystem implementation End
	
};
