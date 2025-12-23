// Fill out your copyright notice in the Description page of Project Settings.


#include "Lightsout/Core/Minigames/HideAndSeek/HideAndSeekGameMode.h"


AHideAndSeekGameMode::AHideAndSeekGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Delegated call to Super::ctor(const FObjectInitializer&), game has started: %d"), ANSI_TO_TCHAR(__FUNCTION__), HasMatchStarted());
}

void AHideAndSeekGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AHideAndSeekGameMode::StartPlay() 
{
	Super::StartPlay();
	UE_LOG(LogTemp, Warning, TEXT("NumPlayers: %d"), GetNumPlayers());
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Delegated call to Super::StartPlay()"), ANSI_TO_TCHAR(__FUNCTION__));
}

// TODO: Check if this is multiple v-table dereferences for invoking the function ( probably not because this is not a function pointer && Super::Func is an identifier itself ).
bool AHideAndSeekGameMode::HasMatchStarted() const
{
	return Super::HasMatchStarted();
}

bool AHideAndSeekGameMode::HasMatchEnded() const 
{
	return Super::HasMatchEnded(); 
}

void AHideAndSeekGameMode::Invoke()
{
	EndDelegate.ExecuteIfBound(); 
}