// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameMapsSettings.h"
//#include "LightsOut/Core/Minigames/Containers/TMinigame.h"
#include "LightsOut/Core/Minigames/HideAndSeek/HideAndSeekGameMode.h"
#include "MinigameManager.generated.h"

/**
 * 
 */

namespace LOUT
{
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
}

UCLASS()
class LIGHTSOUT_API UMinigameManager : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	
	bool bInMinigame;
	
	FTimerHandle MinigameTimerHandle;
	
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override; 
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override; 
	
	void	BeginMinigame(LOUT::FMinigame& InMinigame);
	
	void	OnMinigameEnd(LOUT::FMinigame& InMinigame);
	
	void	BindGameModeDelegates(); 

	bool	InClientContext() const  { return ( GetWorld()->GetNetMode() == NM_Client); };
	
	void ChangeMinigame();
	
public:
	
	inline bool InMinigame() const { return bInMinigame; }
	
};
