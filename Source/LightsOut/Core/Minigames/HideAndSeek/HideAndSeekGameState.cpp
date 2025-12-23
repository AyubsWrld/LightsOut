// Fill out your copyright notice in the Description page of Project Settings.


#include "Lightsout/Core/Minigames/HideAndSeek/HideAndSeekGameState.h"

#include "LocalizationDescriptor.h"

EGameState	AHideAndSeekGameState::GetGameState()	const
{
	return {};
}

bool AHideAndSeekGameState::HasMatchStarted() const
{
	UE_LOG(LogTemp, Warning, TEXT("Game Started"));
	UE_LOG(LogTemp, Warning, TEXT("[%s]: #Players = %lu "), ANSI_TO_TCHAR(__FUNCTION__), PlayerArray.Num());
	return Super::HasMatchStarted(); 
}

void AHideAndSeekGameState::SetGameState(EGameState State)	
{
	GameState = State;
	return; 
}

void AHideAndSeekGameState::HandleBeginPlay()
{
	Super::HandleBeginPlay();
	SetGameState(EGameState::GS_PreTimer);
	UE_LOG(LogTemp, Warning, TEXT("Game Started"));
	UE_LOG(LogTemp, Warning, TEXT("[%s]: #Players = %lu "), ANSI_TO_TCHAR(__FUNCTION__), PlayerArray.Num());
}
