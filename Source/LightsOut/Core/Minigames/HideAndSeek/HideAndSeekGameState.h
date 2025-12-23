// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HideAndSeekGameState.generated.h"

/**
 * 
 */


/* This should be polymorphic */
enum class EGameState
{
	GS_Undefined,
	GS_PreTimer,
	GS_TimerIsRunning,
	GS_DispatchAuditor,
	GS_End,
	GS_Max,
};

UCLASS()
class LIGHTSOUT_API AHideAndSeekGameState : public AGameStateBase
{
	GENERATED_BODY()

	EGameState	GameState{}; 
	
	virtual void HandleBeginPlay() override;
	
	virtual bool HasMatchStarted() const override;
	
public:

	
	EGameState	GetGameState()	const ; 
	
	void		SetGameState(EGameState State);
};
