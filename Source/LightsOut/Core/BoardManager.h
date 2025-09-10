// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "BoardManager.generated.h"

/**
 * 
 */

UCLASS()
class LIGHTSOUT_API UBoardManager : public UWorldSubsystem
{
	GENERATED_BODY()

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION(Reliable, NetMulticast)
	void MulticastGreetPlayers() const; 

	TArray<APlayerState*> PlayerStates;

	APlayerController& ActivePlayer() const;

	/* ------------------------------------ Setup Functions ----------------------------------- */
	
	void BindPlayerControllers();

public:

	UFUNCTION(Reliable, Server)
	void ServerHandleRequest(const APlayerState* Player) const; 


};
