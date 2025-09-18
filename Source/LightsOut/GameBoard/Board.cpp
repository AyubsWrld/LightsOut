#include "LightsOut/Core/BoardManager.h"


/*----------------------------------------------- UTILS ---------------------------------------------*/

void LogFVector(const FVector& F) 
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: (%f,%f,%f)"), ANSI_TO_TCHAR(__FUNCTION__), F.X, F.Y, F.Z );
}


/*----------------------------------------------- UTILS ---------------------------------------------*/
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
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));

	/* Box Collision Volume Setup */
	BoxCollider->SetupAttachment(RootComponent);
	BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollider->SetGenerateOverlapEvents(true);
	BoxCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	BoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this,&ABoard::HandleBeginOverlap);
	BoxCollider->OnComponentEndOverlap.AddDynamic(this, &ABoard::HandleEndOverlap);


	SetReplicates(true);
	SetReplicateMovement(true);

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

	SetStartingPoints();
	SpawnPlayers();
}

void ABoard::HandleBeginOverlap(
	UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult
)
{
	if (ALightsOutCharacter* Player{ Cast<ALightsOutCharacter>(OtherActor) }; Player)
	{ 
		if (PlayersInColliderVolume.Num() == 0) { bQueryIsInterest = true; }
		PlayersInColliderVolume.AddUnique(TObjectPtr<ALightsOutCharacter>(Player));
	}
}

void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bQueryIsInterest)
	{
		if (IsViewInterest())
		{
			//Highlight();
		}
	}
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

			// Always set replciated
			TileMesh->SetIsReplicated(true);
			TileMesh->SetNetAddressable();

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
			Tile.Center = TileLocation;

			// add to collections
			Tiles.Add(Tile);
			TileMap.emplace(CCoords, Tile);

			TileIndex++;
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
			const FTile& Tile = StartTiles[i];

			UStaticMeshComponent* PMesh = NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("PlayerPiece_%d"), i));
			PMesh->SetupAttachment(RootComponent);

			PMesh->SetIsReplicated(true);          // important
			PMesh->SetNetAddressable();            // gives it a stable NetGUID
			AddInstanceComponent(PMesh);           // make it an instance subobject of the actor

			// Copy the mesh and material settings from PlayerPeceModel
			if (PlayerPieceModel)
			{
				PMesh->SetStaticMesh(PlayerPieceModel);
				PMesh->SetMaterial(0, PlayerPieceModel->GetMaterial(0));
			}

			FVector StartPos{ Tile.Center + RootComponent->GetComponentLocation() };
			PMesh->SetWorldLocation(FVector{ StartPos + FVector{0.0f, 0.0f, 5.0f} });
			PMesh->SetWorldScale3D(FVector{ 1.f, 1.f, 1.f });

			PMesh->RegisterComponent();

			//FPlayerPiece P{ Mesh: PMesh, Coordiantes:  };

			PlayerPieces.Emplace(PMesh);
			PlayerPiecesMetadata[PMesh] = FPlayerPieceMetadata{ .Coordinates = Tile.Coordinates, .Location = PMesh->GetComponentLocation() };

		}
	}
}

void ABoard::SetStartingPoints()
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
			std::pair<int32, int32> Coord{ *bit };
			// Sanity check that coordinates actually exist. Checks hashmap 
			if (decltype(TileMap)::const_iterator Tile{ TileMap.find(Coord) }; Tile != TileMap.end())
				// Add to start tiles if it the coordinates exist in tile map (it should).
				StartTiles.Emplace(Tile->second);
		}
	}
}
const FVector& ABoard::GetTileLocation(const std::pair<int32, int32>& Coordinates) const
{
	if (Coordinates.first < 0 || Coordinates.first >= HEIGHT || Coordinates.second < 0 || Coordinates.second >= WIDTH)
		return FVector::ZeroVector;
	if (decltype(TileMap)::const_iterator it{ TileMap.find(Coordinates) }; it != TileMap.end())
	{
		//return it->second.Center + RootComponent->GetComponentLocation();
		return it->second.Center;
	}
	else
	{
		return FVector::ZeroVector;
	}
}

void ABoard::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ALightsOutCharacter* Player{ Cast<ALightsOutCharacter>(OtherActor) }; Player)
	{
		PlayersInColliderVolume.RemoveSingle(TObjectPtr<ALightsOutCharacter>(Player));
		if (PlayersInColliderVolume.Num() == 0) { bQueryIsInterest = false; }
	}
}

void ABoard::SetTileColor(const std::pair<int32, int32>& Coordinates, FLinearColor NewColor)
{
	auto TileIt = TileMap.find(Coordinates);
	if (TileIt == TileMap.end()) return;

	UStaticMeshComponent* TileMesh = TileIt->second.MeshComponent;
	if (!TileMesh) return;

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

void ABoard::Highlight()
{
	DrawDebugBox(GetWorld(), RootComponent->GetComponentLocation(), FVector{100.0f, 100.0f, 100.0f }, FColor::Red, false, 1.0f, 0.0f, 5.0f);
};


void ABoard::Interact(APlayerState* Player)
{
	if (PlayerPieces.IsEmpty())
		return;

	/* Reference to board should be cached */
	UBoardManager* BoardManager{ GetWorld()->GetSubsystem<UBoardManager>() };
	BoardManager->ServerHandleRequest(Player, this);
}
bool ABoard::IsViewInterest()
{
	FHitResult HitResult; 
	FCollisionQueryParams Parameters; 

	for (TObjectPtr<ALightsOutCharacter> Player : PlayersInColliderVolume)
	{
		if (const UCameraComponent* const PlayerCamera{ Player->GetFirstPersonCameraComponent() }; PlayerCamera)
		{
			Parameters.AddIgnoredActor(Player);
			FVector    Start{ PlayerCamera->GetComponentLocation() };
			GetWorld()->LineTraceSingleByChannel(
				HitResult,
				Start,
				Start + (PlayerCamera->GetForwardVector() * 1000.0f),
				ECollisionChannel::ECC_WorldDynamic,
				Parameters
			);

			if (UPrimitiveComponent* P{ HitResult.GetComponent() }; P )
			{
				DrawDebugBox(GetWorld(), P->GetComponentLocation(), FVector{50.0f, 50.0f, 10.0f }, FColor::Cyan , false, 0.1f, 0.0f, 2.0f);
			}

			if (HitResult.bBlockingHit && IsValid(HitResult.GetActor()))
			{
				if (ABoard* Board{ Cast<ABoard>(HitResult.GetActor()) }; Board)
				{
					return true;
				}
			}
		}
	}
	return false;
}
void cb(AActor* A = nullptr)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Invocable stub called"), ANSI_TO_TCHAR(__FUNCTION__));
}
