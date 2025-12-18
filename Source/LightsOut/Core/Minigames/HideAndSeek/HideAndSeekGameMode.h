// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HideAndSeekGameMode.generated.h"

/**
 * 
 */

// What is LIGHTSOUT_API macro? 

/*
namespace MatchState
{
	extern ENGINE_API const FName EnteringMap;			// We are entering this map, actors are not yet ticking
	extern ENGINE_API const FName WaitingToStart;		// Actors are ticking, but the match has not yet started
	extern ENGINE_API const FName InProgress;			// Normal gameplay is occurring. Specific games will have their own state machine inside this state
	extern ENGINE_API const FName WaitingPostMatch;		// Match has ended so we aren't accepting new players, but actors are still ticking
	extern ENGINE_API const FName LeavingMap;			// We are transitioning out of the map to another location
	extern ENGINE_API const FName Aborted;				// Match has failed due to network issues or other problems, cannot continue

	// If a game needs to add additional states, you may need to override HasMatchStarted and HasMatchEnded to deal with the new states
	// Do not add any states before WaitingToStart or after WaitingPostMatch
}
*/

UCLASS()
class LIGHTSOUT_API AHideAndSeekGameMode : public AGameModeBase
{
	GENERATED_BODY()

	/**
	 * Initialize the game.
	 * The GameMode's InitGame() event is called before any other functions (including PreInitializeComponents() )
	 * and is used by the GameMode to initialize parameters and spawn its helper classes.
	 * @warning: this is called before actors' PreInitializeComponents.
	 * @warning: Overriding without calling Super() causes exception violations. See implementation. 
	 */

	/* AGameModeBase Interface */ 
	AHideAndSeekGameMode(const FObjectInitializer& ObjectInitializer);
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	virtual bool HasMatchStarted() const override;
	virtual bool HasMatchEnded() const override;

	/*
	   @purpose:		Simple function to begin the timer for the players to hide, Updates associated 
						AGameState.
										   return
	
	   @code:         S_OK                    Partition was created successfully. 
	
	   @notes:        This function delegates platform logic and does not itself implement
					  partition logic directly.
	
	*/	
	
	void BeginTimer();
	void InvokeCallback();
	
public:
	
	DECLARE_DELEGATE(FMinigameEndDelegate);
	FMinigameEndDelegate EndDelegate;

	FMinigameEndDelegate& GetEndDelegate() { return EndDelegate; };
};
