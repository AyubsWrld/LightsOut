// Fill out your copyright notice in the Description page of Project Settings.

#include "MinigameManager.h"

bool UMinigameManager::ShouldCreateSubsystem(UObject* Outer) const 
{
	const UWorld* World = Cast<UWorld>(Outer);
	if (!World || !World->IsGameWorld()) return false;

	const ENetMode  NetMode = World->GetNetMode();
	return (NetMode == NM_DedicatedServer || NetMode == NM_ListenServer);
}

/* This is called everytime a new level is loaded in */
void UMinigameManager::OnWorldBeginPlay(UWorld& InWorld)
{
	UE_LOG(LogTemp, Warning, TEXT("[UMinigameManager]: OnWorldBeginPlay Invoked"));
	UE_LOG(LogTemp, Warning, TEXT("[UMinigameManager]: World UUID: %d"), InWorld.GetUniqueID());

	/* Change this to an interface */
	if ( AHideAndSeekGameMode* GameModeReference{ Cast<AHideAndSeekGameMode>(GetWorld()->GetAuthGameMode())}  ; GameModeReference )
	{
		AHideAndSeekGameMode::FMinigameEndDelegate& DelegateHandle{GameModeReference->GetEndDelegate()};
		DelegateHandle.BindLambda([]() -> void { UE_LOG(LogTemp, Error, TEXT("[%s]: Callback invoked"), ANSI_TO_TCHAR(__FUNCTION__));});
		GetWorld()->GetTimerManager().SetTimer(
			MinigameTimerHandle,
			[&](){ ChangeMinigame(); },
			10.0f,
			false
			);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not reference the correct game mode"));
	}
	if (InWorld.GetNetMode() == ENetMode::NM_Client)
		return;
}

void UMinigameManager::ChangeMinigame()
{
	UE_LOG(LogTemp, Warning, TEXT("[UMinigameManager]: ChangeMinigame"));
	GetWorld()->ServerTravel(
		"HideAndSeek",
		false,
		false
		);
	return;
}

void UMinigameManager::TestTimerDelegate() const 
{
	UE_LOG(LogTemp, Warning, TEXT("[UMinigameManager]: TestTimerDelegate Invoked"));
	if ( UWorld* World{ GetWorld() } ; World )
	{
		const auto GameState = World->GetGameState(); 
		//UE_LOG(LogTemp, Log, TEXT("Currently running for: %f"));
	}
}

void UMinigameManager::BeginMinigame(FMinigame& InMinigame)
{
	check(!bInMinigame && "Invalid state, called when minigame is currently in session")
	bInMinigame = true;
	return;
}

void UMinigameManager::OnMinigameEnd(FMinigame& InMinigame)
{
	UE_LOG(LogTemp, Warning, TEXT("[UMinigameManager]: ChangeMinigame"));
	GetWorld()->GetTimerManager().SetTimer(
		MinigameTimerHandle,
		this,
		&UMinigameManager::ChangeMinigame,
		10.0f,
		false
	);
	
	return;
}
