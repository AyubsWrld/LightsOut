#include "Tile.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMesh.h"

ATile::ATile()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

void ATile::BeginPlay()
{
    Super::BeginPlay();
}

void ATile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


void ATile::Invocable(AActor* Actor)
{
    
}