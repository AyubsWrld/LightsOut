#include "Board.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

ABoard::ABoard()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("PMC"));
	Mesh->SetupAttachment(RootComponent);
	CreateGrid();
}

void ABoard::BeginPlay()
{
	Super::BeginPlay();
	bReplicates = true;

	// Populate Textures array with assigned materials
	Textures = { Minoris, Majoris, Finalis, Terminus };

}

void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::CreateGrid()
{
	int32 m{};
	for (int32 i{}; i < HEIGHT ; i++)
	{
		for (int32 j{}; j < WIDTH ; j++)
		{
			if (auto c{ BoardConfiguration[i][j] }; c == '#')
			{
				FProcMeshSection MSection{ PMG::AddQuad(*Mesh, m, FVector{j * 100.0f, i * 100.0f, 0.0f}) };
				Tiles.Add(FTile{MSection, nullptr}); 
				FVector Location = MSection.SectionLocalBox.GetCenter();
				UE_LOG(LogTemp, Warning, TEXT("(%f, %f, %f)"), Location.X, Location.Y, Location.Z);
				if (UWorld* World{ GetWorld() }; World)
				{
					DrawDebugLine(World, Location , Location + FVector{0.0f, 0.0f, 100.0f}, FColor::Red, true, -1.0f, 0, 2.0f);
				}
			}
			m++;
		}
	}

	FVector Location;
	for (int j{} ; auto i : Tiles)
	{
		Location = i.Center;
		UE_LOG(LogTemp, Warning, TEXT("[%s] Section(%d): (%f, %f, %f)"), ANSI_TO_TCHAR(__FUNCTION__), j, Location.X, Location.Y, Location.Z);
		++j;
	}
}

void cb(AActor* A = nullptr)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Invocable stub called"), ANSI_TO_TCHAR(__FUNCTION__));
}
