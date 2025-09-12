// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardManager.h"


void UBoardManager::OnWorldBeginPlay(UWorld& InWorld)
{
	if (InWorld.GetNetMode() != ENetMode::NM_DedicatedServer)
		return;
}

bool UBoardManager::ShouldCreateSubsystem(UObject* Outer) const 
{
	/* We want to allow the player to always have access to this. */
	//if (GetWorld()->GetNetMode() != ENetMode::NM_DedicatedServer && false )
		//return false;
	return true; // Always spawns
}

void UBoardManager::Initialize(FSubsystemCollectionBase& Collection) 
{
	if (GetWorld()->GetNetMode() != ENetMode::NM_DedicatedServer)
		return;

	TDelegate<void(AActor*), FDefaultDelegateUserPolicy> Delegate;

	FOnActorSpawned::FDelegate ActorSpawnedDelegate{ 
		FOnActorSpawned::FDelegate::CreateLambda( [&](AActor* P){
				if (APlayerController* PC{ Cast<APlayerController>(P) }; PC)
					PlayerStates.Add(PC);
			}) 
	};
	FDelegateHandle DispatchFunction{ GetWorld()->AddOnActorSpawnedHandler(ActorSpawnedDelegate) };
}

void UBoardManager::ServerHandleRequest_Implementation(APlayerState* Player) 
{
	if (!Player)
		return;

	TObjectPtr<APlayerController> ActivePlayer{ GetActivePlayer() };
	APlayerState* PSP{ActivePlayer.Get()->PlayerState}; 
	if (PSP && Player)
	{
		if (PSP->GetPlayerId() == Player->GetPlayerId())
		{
			UE_LOG(LogTemp, Warning, TEXT("It is the players turn"));
			UpdateCurrentPlayerIndex();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Wrong Players Turn"));
		}
	}
	//MulticastGreetPlayers();
}

void UBoardManager::MulticastGreetPlayers_Implementation() const
{ 
	UE_LOG(LogTemp, Warning, TEXT("Player Count: %d"), PlayerStates.Num());
}

void UBoardManager::BindPlayerControllers()
{
	FConstPlayerControllerIterator PIT{ GetWorld()->GetPlayerControllerIterator() };
	for (; PIT; ++PIT)
	{
	}
}

TObjectPtr<APlayerController> UBoardManager::GetActivePlayer() const
{
	if (!PlayerStates.IsValidIndex(CurrentPlayerIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Returning null"));
		return {};
	}
	return  PlayerStates[CurrentPlayerIndex] ;
}

void UBoardManager::SetActivePlayer(int32 Index) 
{
};

inline void UBoardManager::UpdateCurrentPlayerIndex() 
{
	CurrentPlayerIndex = ++CurrentPlayerIndex % PlayerStates.Num();
	UE_LOG(LogTemp, Warning, TEXT("Current Players Turn: %d"), CurrentPlayerIndex);
};



