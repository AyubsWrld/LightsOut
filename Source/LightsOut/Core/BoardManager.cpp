// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardManager.h"

using TCartesianCoordinates       =        std::pair<std::size_t, std::size_t>;
using TCoordinatesVector          =        std::vector<std::pair<std::size_t, std::size_t>>;

TCoordinatesVector GetValidMoves(
	const TCartesianCoordinates& dimensions,
	const TCartesianCoordinates& location,
	double dist)
{
	TCoordinatesVector V;
	for (std::size_t i{}; i < dimensions.first ; i++)
		for (std::size_t j{}; j < dimensions.second ; j++)
		{
			double dx = static_cast<double>(location.first) - i;
			double dy = static_cast<double>(location.second) - j;
			if (std::sqrt(dx * dx + dy * dy) <= dist) V.push_back(std::pair<std::size_t, std::size_t>{i, j});
		}
	return V;
}
void UBoardManager::UpdateBoardState()
{
	using enum EBoardState;
	switch (State)
	{
		case EBS_AwaitingRoll:
			State = EBS_AwaitingMove;
			break;
		case EBS_AwaitingMove:
			State = EBS_AwaitingRoll;
			break;
	}
};

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

	if (GetBoardState() == EBoardState::EBS_AwaitingRoll)
	{
		RenderIndicators(Board);
		UpdateBoardState();
		return;
	}

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
	UpdateBoardState();
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
void UBoardManager::UpdateCurrentPlayerIndex() 
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
		if (TObjectPtr<APlayerState> PState{ PC->PlayerState }; PState)
			return Player->GetPlayerId() == PState->GetPlayerId();
	return false;
}

/*
   @purpose:      Moves players piece and replicates it across all clients. 

   @param:        [in]                     FVector       Location to move piece to 

   @param:        [in]                     ABoard*       Reference to gameboard actor.

   @notes:        In progress, currently movement replication breaks when called from the
				  BoardManager.

   @todo:         Check whether location is valid. and falls within board dimensions. 

   @todo:         Have a better way of translating Location to 3*3 coordinates.
*/

void UBoardManager::MulticastMovePiece_Implementation(FVector Location, ABoard* Board)
{
	using          TPieceMap = std::unordered_map<UStaticMeshComponent*, FPlayerPieceMetadata>;

	if (!Board)
		return;

	UWorld* World = Board->GetWorld();
	if (!World)
		return;

	UStaticMeshComponent*         Piece                  { Board->PlayerPieces[CurrentPlayerIndex] };

	if (!Piece) return;

	FPlayerPieceMetadata&         PlayerPieceMetadata    { Board->PlayerPiecesMetadata[Piece]   };
	FVector                       TmpLocation            {};

	for (std::size_t i{}; i < 5; i++)
	{
		for (std::size_t j{}; j < 5; j++)
		{
			/* Todo cache this once */
			TmpLocation = Board->GetTileLocation({ i,j }) + Board->GetRootComponent()->GetComponentLocation();
			if(TmpLocation == Location)
			{
				UE_LOG(
					LogTemp,
					Warning,
					TEXT("Found match: (%f,%f,%f) == (%f,%f,%f)"),
					TmpLocation.X,
					TmpLocation.Y,
					TmpLocation.Z,
					Location.X,
					Location.Y,
					Location.Z
				);
				PlayerPieceMetadata.Coordinates = { i,j };
				PlayerPieceMetadata.Location    = Location;
			}
		}
	}
	Piece->SetWorldLocation(Location);
}

/*
   @purpose:      Performs Dice roll and renders indicator of valid moves a player can 
				  make based on the moves result of the dice roll. 

   @param:        [in]                     ABoard*       Reference to gameboard actor.

   @notes:        In progress, currently movement replication breaks when called from the
				  BoardManager.

   @todo:         Seperate concerns of generating number from rendering indicator.
*/


void UBoardManager::RenderIndicators(ABoard* Board)
{
	int32 R1{ FMath::RandRange(1,6) };
	int32 R2{ FMath::RandRange(1,6) };
	int32 AllowedDistance{ FMath::Abs(R2 - R1) };


	FVector                      ValidMoveLoc      {}                                         ;
	UStaticMeshComponent*        PlayerPiece       { Board->PlayerPieces[CurrentPlayerIndex] };
	FPlayerPieceMetadata         PlayerPieceMeta   { Board->PlayerPiecesMetadata[PlayerPiece]};

	
	TCoordinatesVector ValidMoves{
		GetValidMoves({5u, 5u}, PlayerPieceMeta.Coordinates , AllowedDistance)
	};

	for (std::size_t i{}; i < std::size(ValidMoves); i++)
	{
		ValidMoveLoc = Board->GetTileLocation(std::pair{ static_cast<std::size_t>(ValidMoves[i].first), static_cast<std::size_t>(ValidMoves[i].second) }) + Board->GetRootComponent()->GetComponentLocation();
		DrawDebugBox(
			Board->GetWorld(),
			ValidMoveLoc	,
			{ 50.0f, 50.0f, 10.0f },
			FColor::Green,
			false,
			5.0f,
			0,
			2
		);
	}
}

