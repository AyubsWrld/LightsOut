// Fill out your copyright notice in the Description page of Project Settings.

#include "MinigameManager.h"
#include "GameFramework/GameStateBase.h"


enum class EWinCondition
{
	WC_UNDEFINED,
	WC_TimeBased,
	WC_EliminationBased,
	WC_PuzzleBased,
	WC_MAX
};

enum class EMinigameDifficulty
{
	MGD_UNDEFINED,
	MGD_Easy,
	MGD_Medium,
	MGD_Hard,
	MGD_Insane,
};

struct FMinigame
{
	AGameModeBase*		GameMode;
	FName				LevelIdentifier;
	EWinCondition		WinCondition;
	EMinigameDifficulty Difficulty;
	
	FMinigame()		=	default;
	~FMinigame()	=	default;
	
	FMinigame(const FMinigame& InMinigame) = delete;
	FMinigame(const FMinigame&& InMinigame) = delete;
	
	FMinigame& operator=(const FMinigame& InMinigame) noexcept	=	delete;
	FMinigame& operator=(FMinigame&& InMinigame) noexcept		=	delete;
	
	void BeginMinigame();
	void EndMinigame();
};

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
	
	if (const AGameStateBase* GameState = GetWorld()->GetGameState(); GameState)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UMinigameManager]: Found GameState"));
		UE_LOG(LogTemp, Warning, TEXT("[UMinigameManager]: Number Players: %lu"), GameState->PlayerArray.Num());
	}
	
	// Start 10 second timer
	GetWorld()->GetTimerManager().SetTimer(
		MinigameTimerHandle,
		this,
		&UMinigameManager::ChangeMinigame,
		10.0f,
		false // Don't loop
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
