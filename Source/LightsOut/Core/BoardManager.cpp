// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardManager.h"
void UBoardManager::OnWorldBeginPlay(UWorld& InWorld)
{
	if (InWorld.GetNetMode() != ENetMode::NM_DedicatedServer)
		return;
}

bool UBoardManager::ShouldCreateSubsystem(UObject* Outer) const 
{
	return true; 
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

/*
   @purpose:      Server side RPC invoked when a player interacts with the board. This 
				  routine checks whether it is the invoking players turn, if so it conducts
				  and a line trace, moving the players piece to the location of the line trace.

   @param:        [in]                     APlayerState* Invoking players PlayerState used to 
										   determine whether it is the players turn or not.

   @param:        [in]                     ABoard*       Reference to gameboard actor.


   @notes:        In progress, this routine shoudl be split up into different parts as it currently
				  handles the LineTracing, Player Validation, and call to mutlicast delegate for moving 
				  the players piece.

   @todo:         
*/
void UBoardManager::ServerHandleRequest_Implementation(APlayerState* Player, ABoard* Board)
{

	UWorld* World{ Board->GetWorld() };
	APlayerController* PC{ GetActivePlayer() };

	if (!World || PC->PlayerState->GetPlayerId() != Player->GetPlayerId() || !Player ) return;

	if (ALightsOutCharacter* Character{ Cast<ALightsOutCharacter>(PC->GetPawn()) }; Character)
	{
		UCameraComponent* Camera = Character->GetFirstPersonCameraComponent();
		FRotator ControlRotation = Character->GetControlRotation();
		FVector Start = Camera->GetComponentLocation();
		FVector End = Start + (ControlRotation.Vector() * 1000.0f);
		FHitResult HitResult;
		FCollisionQueryParams Params;

		Params.AddIgnoredActor(Character);

		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECollisionChannel::ECC_WorldDynamic,
			Params	
		);
		
		if (UPrimitiveComponent* P{ HitResult.GetComponent() }; P )
		{
			FVector Location = P->GetComponentLocation();
			MulticastMovePiece(Location, Board);
		}
	}

	/* Update current players turn. */

	UpdateCurrentPlayerIndex();
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

/*
   @purpose:      Updates current players index which points to the playeys array. 
				  Calls to GetActivePlayer rely on this index for retreiving the 
				  player whose turn it is from the players container.

   @notes:        In progress, currently movement replication breaks when called from the
				  BoardManager.

   @todo:         
*/
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

/*
   @purpose:      Moves players piece and replicates it across all clients. 

   @param:        [in]                     FVector       Location to move piece to 

   @param:        [in]                     ABoard*       Reference to gameboard actor.

   @notes:        In progress, currently movement replication breaks when called from the
				  BoardManager.

   @todo:         Check whether location is valid. and falls within board dimensions. 
*/

void UBoardManager::MulticastMovePiece_Implementation(FVector Location, ABoard* Board)
{
	if (!Board)
		return;

	UWorld* World = Board->GetWorld();
	if (!World)
		return;

	UE_LOG(LogTemp, Warning, TEXT("[%s]: (%f,%f,%f)"), ANSI_TO_TCHAR(__FUNCTION__), Location.X, Location.Y, Location.Z );

	UStaticMeshComponent* Piece{ Board->PlayerPieces[CurrentPlayerIndex] };

	if (!Piece) return;

	Piece->SetWorldLocation(Location);

}

