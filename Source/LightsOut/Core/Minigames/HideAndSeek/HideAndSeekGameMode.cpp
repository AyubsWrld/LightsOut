// Fill out your copyright notice in the Description page of Project Settings.


#include "Lightsout/Core/Minigames/HideAndSeek/HideAndSeekGameMode.h"


void AHideAndSeekGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Delegated call to Super::InitGame(FString, FString, FString)"), ANSI_TO_TCHAR(__FUNCTION__));
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
