// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameMapsSettings.h"
#include "MinigameManager.generated.h"

/**
 * 
 */

struct FMinigame;

UCLASS()
class LIGHTSOUT_API UMinigameManager : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	
	bool bInMinigame;
	
	FTimerHandle MinigameTimerHandle;
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override; 
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) ; 
	
	void BeginMinigame(FMinigame& InMinigame);
	
	void OnMinigameEnd(FMinigame& InMinigame);
	
	
public:
	
	void ChangeMinigame();
	
	inline bool IsCurrentlyInMinigame() const { return bInMinigame; }
	
};
