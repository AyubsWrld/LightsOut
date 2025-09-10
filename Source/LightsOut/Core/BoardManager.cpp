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
				if (APlayerController* PC{Cast<APlayerController>(P)}; PC)
					PlayerStates.Add(PC->GetPlayerState<APlayerState>());
			}) 
	};
	FDelegateHandle DispatchFunction{ GetWorld()->AddOnActorSpawnedHandler(ActorSpawnedDelegate) };
}

void UBoardManager::ServerHandleRequest_Implementation(const APlayerState* Player) const
{
	if (!Player)
		return;

	//FUniqueNetIdRepl NetId{ Player->BP_GetUniqueId() };
	//UE_LOG(LogTemp, Warning, TEXT("PlayerID: %d"), Player->GetPlayerID());

	MulticastGreetPlayers();
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



