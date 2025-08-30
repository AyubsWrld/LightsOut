#include "Board.h"
#include "Tile.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

ABoard::ABoard()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("PMC"));
	Mesh->SetupAttachment(RootComponent);

	PMG::AddTriangle(*Mesh, 0, FVector{});
}

void ABoard::BeginPlay()
{
	Super::BeginPlay();
	bReplicates = true;

	// Populate Textures array with assigned materials
	Textures = { Minoris, Majoris, Finalis, Terminus };

	// Spawn the grid after BeginPlay when GetWorld() is available
	SpawnGrid();

    UWorld* World = GetWorld(); 
    if (!World)
        return;

    TArray<FVector> Vertices{ 
        FVector{0.0f, 0.0f, 0.0f},
        FVector{0.0f, 100.0f, 0.0f},
		FVector{100.0f, 0.0f, 0.0f} 
    };

    DrawDebugLine(World, Vertices[0], Vertices[1], FColor::Emerald, true, -1.0f, 0, 2.0f);
}

void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::SpawnGrid_Implementation()
//void ABoard::SpawnGrid()
{
	
}