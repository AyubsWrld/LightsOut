// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "LightsOut/GameBoard/Board.h"
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

	int32 CurrentPlayerIndex{};

	TArray<TObjectPtr<APlayerController>> PlayerStates;
	/* ------------------------------------ Setup Functions ----------------------------------- */
	
	void BindPlayerControllers();

public:

    UFUNCTION(Reliable, NetMulticast)
    void MulticastMovePiece(FVector Location, ABoard* Board);
	UFUNCTION(Reliable, Server)
	[[nodiscard]] void ServerHandleRequest(APlayerState* Player, ABoard* Board, UPrimitiveComponent* Component) ; 

	bool IsPlayersTurn(APlayerState* Player) ; 
	TObjectPtr<APlayerController> GetActivePlayer() const; 

	void SetActivePlayer(int32 Index) ; 

	inline void UpdateCurrentPlayerIndex();

};
