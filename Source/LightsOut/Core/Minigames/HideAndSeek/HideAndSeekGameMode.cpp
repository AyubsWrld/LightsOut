// Fill out your copyright notice in the Description page of Project Settings.


#include "Lightsout/Core/Minigames/HideAndSeek/HideAndSeekGameMode.h"
#include "LightsOut/Core/MinigameManager.h"
#include "GameFramework/GameStateBase.h"


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
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AHideAndSeekGameMode::StartPlay() 
{
	Super::StartPlay();
	UE_LOG(LogTemp, Warning, TEXT("NumPlayers: %d"), GetNumPlayers());
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Delegated call to Super::StartPlay()"), ANSI_TO_TCHAR(__FUNCTION__));
	BeginTimer();
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

void AHideAndSeekGameMode::BeginTimer()
{
	FTimerHandle TimerHandle{};
	
	// FORCEINLINE void SetTimer(FTimerHandle& InOutHandle, FTimerDelegate const& InDelegate, float InRate, const FTimerManagerTimerParameters& InTimerParameters)
	//FORCEINLINE void SetTimer(FTimerHandle& InOutHandle, UserClass* InObj, typename FTimerDelegate::TMethodPtr< UserClass > InTimerMethod, float InRate, bool InbLoop = false, float InFirstDelay = -1.f)
	
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&AHideAndSeekGameMode::InvokeCallback,
		40.0f,
		false,
		false
		);

	/* Invoked imemdiately
	 
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[&](){ EndDelegate.ExecuteIfBound(); },
		40.0f,
		false,
		false
		);
	*/
}

void AHideAndSeekGameMode::InvokeCallback() 
{
	EndDelegate.ExecuteIfBound(); 
}


