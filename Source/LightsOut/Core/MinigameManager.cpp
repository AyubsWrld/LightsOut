// Fill out your copyright notice in the Description page of Project Settings.

#include "MinigameManager.h"
#include "GameFramework/GameStateBase.h"


bool UMinigameManager::ShouldCreateSubsystem(UObject* Outer) const 
{
	const UWorld* World = Cast<UWorld>(Outer);
	if (!World || !World->IsGameWorld()) return false;

	const ENetMode  NetMode = World->GetNetMode();
	return (NetMode == NM_DedicatedServer || NetMode == NM_ListenServer);
}

void UMinigameManager::OnWorldBeginPlay(UWorld& InWorld)
{
	if (InWorld.GetNetMode() == ENetMode::NM_Client)
		return;
		
	UE_LOG(LogTemp, Warning, TEXT("[UMinigameManager]: BeginPlay Called"));

	/*
	if (const AGameStateBase* GameState = GetWorld()->GetGameState(); GameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMinigameManager]: Found GameState"));
		UE_LOG(LogTemp, Warning, TEXT("[UMinigameManager]: Number Players: %lu"), GameState->PlayerArray.Num());
	}
	*/
	
	GetWorld()->GetTimerManager().SetTimer(
		MinigameTimerHandle,
		this,
		&UMinigameManager::TestTimerDelegate,
		10.0f,
		false 
	);
	
	UE_LOG(LogTemp, Warning, TEXT("[UMinigameManager]: Timer set for 10 seconds"));
}

void UMinigameManager::ChangeMinigame()
{
	UE_LOG(LogTemp, Warning, TEXT("[UMinigameManager]: ChangeMinigame"));
	GetWorld()->ServerTravel(
		"TestTransition",
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
		UE_LOG(LogTemp, Log, TEXT("Currently running for: %f"), GameState->CreationTime);
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
	return;
}
