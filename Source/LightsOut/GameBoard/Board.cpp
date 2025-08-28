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
	for (auto Square : Squares)
	{
		if (Square)
		{
			Square->SetMaterial(0, Minoris);
		}
	}
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
					Squares.Add(MeshComp);
				}
			}
		}
	}
}

void ABoard::GetAllStaticMeshComponents()
{
	Squares.Empty();

	TArray<UStaticMeshComponent*> StaticMeshComponents;
	GetComponents<UStaticMeshComponent>(StaticMeshComponents);

	StaticMeshComponents.Sort([](const UStaticMeshComponent& A, const UStaticMeshComponent& B) {
		return A.GetName() < B.GetName();
		});

	Squares = StaticMeshComponents;

	UE_LOG(LogTemp, Warning, TEXT("Found %d static mesh components using GetComponents"), Squares.Num());
}