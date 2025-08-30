#include "Board.h"
#include "Tile.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABoard::BeginPlay()
{
	Super::BeginPlay();
	bReplicates = true;

	// Populate Textures array with assigned materials
	Textures = { Minoris, Majoris, Finalis, Terminus };

	// Spawn the grid after BeginPlay when GetWorld() is available
	SpawnGrid();
}

void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::SpawnGrid_Implementation()
//void ABoard::SpawnGrid()
{
	
}