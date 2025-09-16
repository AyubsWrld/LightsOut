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

void UBoardManager::ServerHandleRequest_Implementation(APlayerState* Player, ABoard* Board, UPrimitiveComponent* Component) 
{
	if (!Player)
		return;

	UWorld* World{ Board->GetWorld() };

	if (!World)
		return;

	if (IsPlayersTurn(Player))
	{
		if (auto P{GetActivePlayer()}; P)
		{
			UE_LOG(LogTemp, Warning, TEXT("Getting active player"));
			if (APlayerState* PlayerState{ P->PlayerState.Get() }; PlayerState)
			{
				UE_LOG(LogTemp, Warning, TEXT("Player is well defined"));
				if (ALightsOutCharacter* LP{ Cast<ALightsOutCharacter>(P->GetPawn()) }; LP)
				{
					UE_LOG(LogTemp, Warning, TEXT("Player is well defined"));
					UCameraComponent* Camera = LP->GetFirstPersonCameraComponent();
					DrawDebugLine(
						World,
						Camera->GetComponentLocation(),
						Camera->GetComponentLocation() + (Camera->GetForwardVector() * 1000.0f),
						FColor::Red,
						true,
						-1.0f,
						0,
						3.00f
					);
				}
			}
		}
		if (Component)
		{
			UE_LOG(LogTemp, Warning, TEXT("Component (%s) hit"), *Component->GetFName().ToString());
			MulticastMovePiece(Component->GetComponentLocation(), Board);
		}
	}
	return;
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

/* Fix control paths for this */
bool UBoardManager::IsPlayersTurn(APlayerState* Player)
{
	if (!Player) 
		return false;
	if (TObjectPtr<APlayerController> PC{ GetActivePlayer() }; PC)
		if (TObjectPtr<APlayerState> State{ PC->PlayerState }; State)
		{
			UE_LOG(LogTemp, Warning, TEXT("Comparing PlayerID (%d) to Active Player (%d)"), Player->GetPlayerId(), State->GetPlayerId());
			return Player->GetPlayerId() == State->GetPlayerId();
		}
	return false;
}


void UBoardManager::MulticastMovePiece_Implementation(FVector Location, ABoard* Board)
{
	if (!Board)
		return;

	UWorld* World{ Board->GetWorld() };
	if (!World)
		return;

	// 1) Input sanity
	if (Location.IsZero()) return;
	if (!Board->PlayerPieces.IsValidIndex(0)) return;

	UStaticMeshComponent* P = Board->PlayerPieces[0];

	if (!P) return;

	// 2) Convert tile-local -> world (board root space -> world)

	USceneComponent* RootComponent{ Board->GetRootComponent() };
	const FTransform RootXf = RootComponent ? RootComponent->GetComponentTransform() : Board->GetActorTransform();
	const FVector WorldTarget = RootXf.TransformPosition(Location) + FVector(0.0, 0.0, 5.0);


	/*
	P->SetHiddenInGame(false);
	P->SetVisibility(true, true);
	P->SetRenderInMainPass(true);
	P->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	P->SetCollisionResponseToAllChannels(ECR_Block);
	P->SetRelativeScale3D(FVector(0.035f)); 
	*/

	// 4) Move it
	const FVector CurrentPos = P->GetComponentLocation();

	P->SetWorldLocation(WorldTarget, /*bSweep=*/true);

}



