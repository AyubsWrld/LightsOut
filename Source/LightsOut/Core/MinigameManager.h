// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameMapsSettings.h"
#include "LightsOut/Core/Minigames/HideAndSeek/HideAndSeekGameMode.h"
#include "MinigameManager.generated.h"

/**
 * 
 */

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
	
	void TestTimerDelegate() const;
	
	
public:
	
	void ChangeMinigame();
	
	inline bool IsCurrentlyInMinigame() const { return bInMinigame; }
	
};
