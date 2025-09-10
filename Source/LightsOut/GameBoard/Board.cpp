#include "Board.h"

void ABoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the PlayerPieces array so all clients see the same player pieces
	DOREPLIFETIME(ABoard, PlayerPieces);
	// DOREPLIFETIME(ABoard, Textures);
}

ABoard::ABoard()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("PMC"));
	Mesh->SetupAttachment(RootComponent);
	CreateGrid();
	SetReplicates(true);
	SetReplicateMovement(true);
}
std::array<std::pair<int32, int32>, 4> ABoard::GetBoardBounds()
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
	DebugStartingPoints();
	SpawnPlayers();
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

void ABoard::SpawnPlayers()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn Players called"));
	UE_LOG(LogTemp, Warning, TEXT("Start tiles amount %d"), StartTiles.Num());
	UWorld* World{ GetWorld() };
	if (!World || StartTiles.IsEmpty())
		return;
	else
	{
		for (int32 i = 0; i < StartTiles.Num(); i++)
		{
			const auto& Tile = StartTiles[i];

			// Create a NEW static mesh component for each player at runtime
			UStaticMeshComponent* PMesh = NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("PlayerPiece_%d"), i));
			PMesh->SetupAttachment(RootComponent);

			PMesh->SetIsReplicated(true);          // important
			PMesh->SetNetAddressable();            // gives it a stable NetGUID
			AddInstanceComponent(PMesh);           // make it an instance subobject of the actor

			// Copy the mesh and material settings from PlayerPieceModel
			if (PlayerPieceModel)
			{
				PMesh->SetStaticMesh(PlayerPieceModel);
				PMesh->SetMaterial(0, PlayerPieceModel->GetMaterial(0));
			}

			FVector StartPos{ Tile.Center + RootComponent->GetComponentLocation() };
			PMesh->SetWorldLocation(FVector{ StartPos + FVector{0.0f, 0.0f, 5.0f} });
			PMesh->SetWorldScale3D(FVector{ 0.035f, 0.035f, 0.035f });

			// Register the component so it gets properly updated
			PMesh->RegisterComponent();


			PlayerPieces.Emplace(PMesh);
		}
	}
}

void ABoard::TestGrid()
{
	decltype(TileMap)::const_iterator bit{ TileMap.begin() };
	decltype(TileMap)::const_iterator eit{ TileMap.end() };
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
		decltype(SPoints)::const_iterator bit{ SPoints.begin() };
		decltype(SPoints)::const_iterator eit{ SPoints.end() };
		for (; bit != eit; bit++)
		{
			auto Coord{ *bit };
			if (decltype(TileMap)::const_iterator Tile{ TileMap.find(Coord) }; Tile != TileMap.end())
			{
				/* decltype(Tile) -> TTuple<std::pair<int32,int32>, FTile> */
				StartTiles.Emplace(Tile->second);
				FVector Start{ Tile->second.Center + RootComponent->GetComponentLocation() };
				FVector End{ Start + FVector{0.0f, 0.0f, 100.0f} };
				DrawDebugLine(World, Start, End, FColor::Red, true, -1.0f, 0, 2.0f);
			}
		}
	}
}

const FVector& ABoard::GetTileLocation(const std::pair<int32, int32>& Coordinates) const
{
	// FIXED: Check WIDTH for the second coordinate instead of HEIGHT
	if (Coordinates.first < 0 || Coordinates.first >= HEIGHT || Coordinates.second < 0 || Coordinates.second >= WIDTH)
		return FVector::ZeroVector;
	if (decltype(TileMap)::const_iterator it{ TileMap.find(Coordinates) }; it != TileMap.end())
	{
		return it->second.Center;
	}
	else
	{
		return FVector::ZeroVector;
	}
}

void ABoard::MulticastMovePiece_Implementation(FVector Location)
{
	UE_LOG(LogTemp, Warning, TEXT("MovePiece: local tile=(%f,%f,%f)"), Location.X, Location.Y, Location.Z);

	// 1) Input sanity
	if (Location.IsZero())
	{
		UE_LOG(LogTemp, Error, TEXT("Location is zero (GetTileLocation failed)."));
		return;
	}
	if (!PlayerPieces.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerPieces[0] invalid."));
		return;
	}

	UStaticMeshComponent* P = PlayerPieces[0];
	if (!P)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player piece doesn't exist"));
		return;
	}

	// 2) Convert tile-local -> world (board root space -> world)
	const FTransform RootXf = RootComponent ? RootComponent->GetComponentTransform() : GetActorTransform();
	const FVector WorldTarget = RootXf.TransformPosition(Location) + FVector(0.0, 0.0, 5.0);

	// 3) Make sure the piece is visible & interactable
	P->SetHiddenInGame(false);
	P->SetVisibility(true, true);
	P->SetRenderInMainPass(true);
	P->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	P->SetCollisionResponseToAllChannels(ECR_Block);
	P->SetRelativeScale3D(FVector(0.035f)); // or temporarily bigger while debugging

	// 4) Move it
	const FVector CurrentPos = P->GetComponentLocation();
	UE_LOG(LogTemp, Warning, TEXT("Current=(%f,%f,%f)  Target(world)=(%f,%f,%f)"),
		CurrentPos.X, CurrentPos.Y, CurrentPos.Z, WorldTarget.X, WorldTarget.Y, WorldTarget.Z);

	P->SetWorldLocation(WorldTarget, /*bSweep=*/true);

	// 5) Loud debug viz at the target
	if (UWorld* W = GetWorld())
	{
		DrawDebugBox(W, WorldTarget, FVector(6, 6, 6), FColor::Yellow, /*bPersist=*/true, /*LifeTime=*/10.f, /*DepthPri=*/0, /*Thickness=*/2.f);
		DrawDebugLine(W, WorldTarget, WorldTarget + FVector(0, 0, 120), FColor::Red, true, 10.f, 0, 2.f);
		DrawDebugString(W, WorldTarget + FVector(0, 0, 130), TEXT("PlayerPiece Target"), nullptr, FColor::White, 10.f, true);
	}

	// 6) Optional: make it red so you notice it
	if (UMaterialInterface* Base = P->GetMaterial(0))
	{
		if (UMaterialInstanceDynamic* Dyn = UMaterialInstanceDynamic::Create(Base, this))
		{
			static const FLinearColor Red = FLinearColor::Red;
			Dyn->SetVectorParameterValue(FName("BaseColor"), Red);
			Dyn->SetVectorParameterValue(FName("Color"), Red);
			Dyn->SetVectorParameterValue(FName("Albedo"), Red);
			Dyn->SetVectorParameterValue(FName("Diffuse"), Red);
			P->SetMaterial(0, Dyn);
		}
	}
}
void ABoard::Interact(APlayerState* Player)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact called"));

	if (PlayerPieces.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerPieces is empty!"));
		return;
	}

	std::pair<int32, int32> TargetCoords{ 3,3 };
	UE_LOG(LogTemp, Warning, TEXT("Trying to get location for coordinates (%d,%d)"), TargetCoords.first, TargetCoords.second);

	const FVector& Location = GetTileLocation(TargetCoords);
	UE_LOG(LogTemp, Warning, TEXT("GetTileLocation returned: (%f,%f,%f)"), Location.X, Location.Y, Location.Z);

	if (auto it = TileMap.find(TargetCoords); it != TileMap.end())
	{
		UE_LOG(LogTemp, Warning, TEXT("Tile found in map at (%d,%d)"), TargetCoords.first, TargetCoords.second);
		UE_LOG(LogTemp, Warning, TEXT("Tile center: (%f,%f,%f)"), it->second.Center.X, it->second.Center.Y, it->second.Center.Z);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Tile NOT found in map at (%d,%d)"), TargetCoords.first, TargetCoords.second);
	}

	UBoardManager* BoardManager = GetWorld()->GetSubsystem<UBoardManager>();
	BoardManager->ServerHandleRequest(Player);
	MulticastMovePiece(Location);
}

void cb(AActor* A = nullptr)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Invocable stub called"), ANSI_TO_TCHAR(__FUNCTION__));
}