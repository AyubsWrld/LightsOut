#include "Board.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

ABoard::ABoard()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("PMC"));
	Mesh->SetupAttachment(RootComponent);
	CreateGrid();
}


std::array<std::pair<int32, int32>,4> ABoard::GetBoardBounds()
{
	return {
		std::pair<int32,int32>{ 0, 0 },
		std::pair<int32,int32>{ 0, HEIGHT - 1 },
		std::pair<int32,int32>{ WIDTH - 1 , 0 },
		std::pair<int32,int32>{ HEIGHT - 1 , WIDTH - 1 }
	};
}

void ABoard::BeginPlay()
{
	Super::BeginPlay();
	bReplicates = true;
	// Populate Textures array with assigned materials
	Textures = { Minoris, Majoris, Finalis, Terminus };
	SpawnPlayers();

	const FVector& Location = RootComponent->GetComponentLocation();
	UWorld* World = GetWorld();
	DebugStartingPoints();
}

void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::CreateGrid()
{
	int32 m{};
	for (int32 i{}; i < HEIGHT; i++)
	{
		for (int32 j{}; j < WIDTH; j++)
		{
			if (auto c{ BoardConfiguration[i][j] }; c == '#')
			{
				// (x,y) Coordinates on the Gameboard. 
				std::pair<int32, int32> CCoords{ i,j };

				// Creates mesh section and returns reference to the newly created mesh section. 
				FProcMeshSection MSection{ PMG::AddQuad(*Mesh, m, FVector{j * 10.0f, i * 10.0f, 0.0f}) };

				// Use MeshSection to inititalize FTile. Explicit Temp Object.
				FTile Tile{ MSection, nullptr, CCoords };

				// FTile::Center set within the constructor for FTile. 
				Tiles.Add(Tile); 

				// Add the tile to the TileMap setting the Coordinates to (i,j) 
				TileMap.emplace(std::pair<int32, int32>{i, j}, Tile); 
			}
			m++;
		}
	}
}


/* Tile Locations not set relatively ? */
void ABoard::SpawnPlayers()
{
	UWorld* World{ GetWorld() };
	if (!World || Tiles.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed: !World || Tiles.IsEmpty()"));
		return;
	}
}

void ABoard::TestGrid()
{
	decltype(TileMap)::const_iterator bit{ TileMap.begin()};
	decltype(TileMap)::const_iterator eit{ TileMap.end()};
	for (; bit != eit; bit++)
		UE_LOG(LogTemp, Warning, TEXT("Tile Coord: (%d, %d)"), bit->first.first, bit->first.second);
}

void ABoard::DebugStartingPoints()
{
	auto World{ GetWorld() }; 
	if (!World)
		return;
	else
	{
		std::array<std::pair<int32, int32>, 4> SPoints{ GetBoardBounds() };
		decltype(SPoints)::const_iterator bit{SPoints.begin()};
		decltype(SPoints)::const_iterator eit{SPoints.end()}; 
		for (; bit != eit; bit++)
		{
			auto Coord{ *bit };
			if (decltype(TileMap)::const_iterator Tile{ TileMap.find(Coord) }; Tile != TileMap.end())
			{
				/* decltype(Tile) -> TTuple<std::pair<int32,int32>, FTile> */
				StartTiles.Emplace(Tile->second);
				FVector Start{Tile->second.Center + RootComponent->GetComponentLocation()};
				FVector End{ Start + FVector{0.0f, 0.0f, 100.0f} };
				UE_LOG(LogTemp, Warning, TEXT("DBL: (%f,%f,%f)"), Start.X, Start.Y, Start.Z);
				DrawDebugLine(World, Start, End, FColor::Red, true, -1.0f, 0, 2.0f);
			}
		}
	}
}

void cb(AActor* A = nullptr)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Invocable stub called"), ANSI_TO_TCHAR(__FUNCTION__));
}