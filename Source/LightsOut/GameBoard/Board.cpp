#include "Board.h"
#include "Tile.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

ABoard::ABoard()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("PMC"));
	Mesh->SetupAttachment(RootComponent);

	FVector Offset{};  // ( 0.0f, 0.0f, 0.0f ) 

	int32 m{};
	for (int32 i{}; i < 25; i++)
	{
		for (int32 j{}; j < 25; j++)
		{
			if( auto c{BoardConfiguration[i][j]}; c == '#')
				PMG::AddQuad(*Mesh, m, FVector{j * 100.0f, i * 100.0f, 0.0f}); 
			m++;
		}
	}

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