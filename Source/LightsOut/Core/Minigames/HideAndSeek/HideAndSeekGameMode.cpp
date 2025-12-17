// Fill out your copyright notice in the Description page of Project Settings.


#include "Lightsout/Core/Minigames/HideAndSeek/HideAndSeekGameMode.h"


AHideAndSeekGameMode::AHideAndSeekGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Delegated call to Super::ctor(const FObjectInitializer&), game has started: %d"), ANSI_TO_TCHAR(__FUNCTION__), HasMatchStarted());
	
	// This should be globally set. 
	// bDelayedStart = false; 

	/*
	// One-time initialization
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
	MatchState = MatchState::EnteringMap;
	EngineMessageClass = UEngineMessage::StaticClass();
	GameStateClass = AGameState::StaticClass();
	MinRespawnDelay = 1.0f;
	InactivePlayerStateLifeSpan = 300.f;
	MaxInactivePlayers = 16;
	*/
}

void AHideAndSeekGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Delegated call to Super::InitGame(FString, FString, FString)"), ANSI_TO_TCHAR(__FUNCTION__));
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AHideAndSeekGameMode::StartPlay() 
{
	Super::StartPlay();
	UE_LOG(LogTemp, Warning, TEXT("NumPlayers: %d"), GetNumPlayers());
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Delegated call to Super::StartPlay()"), ANSI_TO_TCHAR(__FUNCTION__));
}

// TODO: Check if this is multiple dereferences.
bool AHideAndSeekGameMode::HasMatchStarted() const
{
	return Super::HasMatchStarted();
}
