#include "Tile.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

ATile::ATile()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create root component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    // Create static mesh component
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
    Mesh->SetupAttachment(RootComponent);

    // Load a default cube mesh (you can change this to any mesh you prefer)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube"));
    if (CubeMeshAsset.Succeeded())
    {
        Mesh->SetStaticMesh(CubeMeshAsset.Object);
        // Scale the cube to make it more tile-like (flatter)
        Mesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.1f));
    }

    // Enable collision
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);
}

void ATile::BeginPlay()
{
    Super::BeginPlay();
}

void ATile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}