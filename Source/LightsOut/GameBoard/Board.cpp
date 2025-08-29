#include "Board.h"

ABoard::ABoard()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ABoard::BeginPlay()
{
	Super::BeginPlay();

	PopulateSquaresFromChildren();
	bReplicates = true;

	SetStartingPosition(4);
}

void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/* @refactor */
/* Appending "Squares" to member variable squares */
void ABoard::PopulateSquaresFromChildren()
{
	/* Don't change this, this is determined by the board itself which is static and should not be changed*/
	TArray<USceneComponent*> ChildComponents = GetRootComponent()->GetAttachChildren();
	TArray<USceneComponent*> SharedComponent = ChildComponents[0]->GetAttachChildren();

	FName Outline{ TEXT("Outline") }; 
	FName StartTile{ TEXT("StartTile") }; 
	int x{};
	for (USceneComponent* Component : SharedComponent)
	{
		if (UChildActorComponent* ChildActorComp = Cast<UChildActorComponent>(Component))
		{
			if (ChildActorComp->ComponentHasTag(StartTile))
			{
				if (AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(ChildActorComp->GetChildActor()))
				{
					StartTiles.Add(FTile{ *StaticMeshActor });
				}
			}

			if (ChildActorComp->ComponentHasTag(Outline))
				return;
			if (AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(ChildActorComp->GetChildActor()))
			{
				Tiles.Add(FTile{ *StaticMeshActor });
			}
		}
	}
}

void ABoard::SetStartingPosition(int32 PlayerCount)
{
	/* @invariant */
	if (!StartTiles.IsValidIndex(PlayerCount - 1))
		return;
	UWorld* World{ GetWorld() };
	for (const FTile& Tile : StartTiles)
	{
		auto PlayerPieces = World->SpawnActor<AItemBase>(Tile.Mesh->GetActorLocation(), FRotator::ZeroRotator);
	}
}

