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

	UpdateSquares();
}

void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABoard::UpdateSquares()
{
	if (!Minoris)
		return;
	UE_LOG(LogTemp, Warning, TEXT("Updating Squares"));
	for (FTile& Tile: Tiles )
	{
		Tile.Mesh->SetMaterial(0, Minoris);
		switch (Tile.Calamity)
		{
		case ECalamity::EC_Minoris:
			UE_LOG(LogTemp, Warning, TEXT("Minoris"));
			break;
		case ECalamity::EC_Majoris:
			UE_LOG(LogTemp, Warning, TEXT("Majoris"));
			break;
		case ECalamity::EC_Terminus:
			UE_LOG(LogTemp, Warning, TEXT("Terminus"));
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("UNDEFINED"));
			break;
		}
	}
}

void cb()
{
	UE_LOG(LogTemp, Warning, TEXT("TEST"));
}



/* Appending "Squares" to member variable squares */
void ABoard::PopulateSquaresFromChildren()
{
	/* Don't change this, this is determined by the board itself which is static and should not be changed*/
	TArray<USceneComponent*> ChildComponents = GetRootComponent()->GetAttachChildren();
	TArray<USceneComponent*> SharedComponent = ChildComponents[0]->GetAttachChildren();

	FName Outline{ "Outline" }; 
	for (USceneComponent* Component : SharedComponent)
	{
		if (UChildActorComponent* ChildActorComp = Cast<UChildActorComponent>(Component))
		{
			if (ChildActorComp->ComponentHasTag(Outline))
				return;
			if (AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(ChildActorComp->GetChildActor()))
			{
				UStaticMeshComponent* MeshComp = StaticMeshActor->GetStaticMeshComponent();
				if (MeshComp && !StaticMeshActor->ActorHasTag("Outline"))
				{
					Tiles.Add(FTile{*MeshComp});
				}
			}
		}
	}
}
