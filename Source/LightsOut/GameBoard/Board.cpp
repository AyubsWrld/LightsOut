#include "Board.h"

void ABoard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// replicate the PlayerPieces array so all clients see the same player pieces
	DOREPLIFETIME(ABoard, PlayerPieces);
}

ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// replication
	SetReplicates(true);
	SetReplicateMovement(true);

	// Load the plane mesh asset
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (PlaneMeshAsset.Succeeded())
	{
		TileMeshAsset = PlaneMeshAsset.Object;
	}

	// visibility
	CreateGrid();
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

	// Only create grid if it wasn't created in constructor
	if (TileMeshes.Num() == 0)
	{
		CreateGrid();
	}

	DebugStartingPoints();
	SpawnPlayers();
}

void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::CreateGrid()
{
	// Clear existing tiles first
	TileMeshes.Empty();
	Tiles.Empty();
	TileMap.clear();

	int32 TileIndex = 0;
	for (int32 i = 0; i < HEIGHT; i++)
	{
		for (int32 j = 0; j < WIDTH; j++)
		{
			if (auto c = BoardConfiguration[i][j]; c == '#')
			{
				// (x,y) Coordinates on the Gameboard. 
				std::pair<int32, int32> CCoords{ i, j };

				// Create individual static mesh component for this tile
				FString TileName = FString::Printf(TEXT("Tile_%d_%d"), i, j);
				UStaticMeshComponent* TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(*TileName);

				if (!TileMesh)
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to create tile mesh component for tile %d,%d"), i, j);
					continue;
				}

				TileMesh->SetupAttachment(RootComponent);

				if (TileMeshAsset)
				{
					TileMesh->SetStaticMesh(TileMeshAsset);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("TileMeshAsset is null for tile %d,%d"), i, j);
				}

				FVector TileLocation{ j * 100.0f, i * 100.0f, 0.0f }; 
				TileMesh->SetRelativeLocation(TileLocation);
				TileMesh->SetRelativeScale3D(FVector{ 1.0f, 1.0f, 1.0f }); 

				// set up replication only if we're in a networked context
				if (GetWorld() && GetWorld()->GetNetMode() != NM_Standalone)
				{
					TileMesh->SetIsReplicated(true);
					TileMesh->SetNetAddressable();
				}

				// set default material
				if (DefaultTileMaterial)
				{
					TileMesh->SetMaterial(0, DefaultTileMaterial);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("DefaultTileMaterial is null for tile %d,%d"), i, j);
				}

				// store reference to mesh
				TileMeshes.Add(TileMesh);

				// create FTile with reference to the mesh component
				FTile Tile{ TileMesh, CCoords };
				Tile.Center = TileLocation; // Set center manually.. make const later and calculate in constructor.

				// add to collections
				Tiles.Add(Tile);
				TileMap.emplace(CCoords, Tile);

				TileIndex++;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Created %d tiles"), TileIndex);
}

void ABoard::SpawnPlayers()
{
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
			PMesh->SetWorldScale3D(FVector{ 0.25f, 0.25f, 0.25f });

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

void ABoard::SetTileColor(const std::pair<int32, int32>& Coordinates, FLinearColor NewColor)
{
	auto TileIt = TileMap.find(Coordinates);
	if (TileIt == TileMap.end()) return;

	// Get the tile's mesh component
	UStaticMeshComponent* TileMesh = TileIt->second.MeshComponent;
	if (!TileMesh) return;

	// Create or get material instance dynamic
	UMaterialInterface* BaseMaterial = TileMesh->GetMaterial(0);
	if (!BaseMaterial && DefaultTileMaterial)
	{
		BaseMaterial = DefaultTileMaterial;
	}

	if (BaseMaterial)
	{
		UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		if (DynMaterial)
		{
			// Try different common parameter names for color
			DynMaterial->SetVectorParameterValue(FName("BaseColor"), NewColor);
			DynMaterial->SetVectorParameterValue(FName("Color"), NewColor);
			DynMaterial->SetVectorParameterValue(FName("Albedo"), NewColor);
			DynMaterial->SetVectorParameterValue(FName("Diffuse"), NewColor);

			TileMesh->SetMaterial(0, DynMaterial);
		}
	}
}

void ABoard::SetTileMaterial(const std::pair<int32, int32>& Coordinates, UMaterialInterface* NewMaterial)
{
	auto TileIt = TileMap.find(Coordinates);
	if (TileIt == TileMap.end()) return;

	UStaticMeshComponent* TileMesh = TileIt->second.MeshComponent;
	if (!TileMesh || !NewMaterial) return;

	TileMesh->SetMaterial(0, NewMaterial);
}

UStaticMeshComponent* ABoard::GetTileMesh(const std::pair<int32, int32>& Coordinates) const
{
	auto TileIt = TileMap.find(Coordinates);
	if (TileIt != TileMap.end())
	{
		return TileIt->second.MeshComponent;
	}
	return nullptr;
}

void ABoard::MulticastMovePiece_Implementation(FVector Location)
{
	// 1) Input sanity
	if (Location.IsZero()) return;
	if (!PlayerPieces.IsValidIndex(0)) return;

	UStaticMeshComponent* P = PlayerPieces[0];

	if (!P) return;

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
	//UE_LOG(LogTemp, Warning, TEXT("Current=(%f,%f,%f)  Target(world)=(%f,%f,%f)"),CurrentPos.X, CurrentPos.Y, CurrentPos.Z, WorldTarget.X, WorldTarget.Y, WorldTarget.Z);

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
	if (PlayerPieces.IsEmpty())
		return;

	std::pair<int32, int32> TargetCoords{ 3,3 };
	const FVector& Location = GetTileLocation(TargetCoords);

	// Example: Change tile color when interacting
	SetTileColor(TargetCoords, FLinearColor::Green);

	UBoardManager* BoardManager = GetWorld()->GetSubsystem<UBoardManager>();
	BoardManager->ServerHandleRequest(Player);
	MulticastMovePiece(Location);
}

void cb(AActor* A = nullptr)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Invocable stub called"), ANSI_TO_TCHAR(__FUNCTION__));
}
