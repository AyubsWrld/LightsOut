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
			UE_LOG(LogTemp, Warning, TEXT("Hit something: %s, (%f,%f,%f)"), *P->GetFName().ToString(), Location.X, Location.Y, Location.Z);
			MulticastMovePiece(Location, Board);
		}
	}
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

	UWorld* World = Board->GetWorld();
	if (!World)
		return;

}



