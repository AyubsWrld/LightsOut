#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "LightsOut/Items/ItemBase.h"
#include "Tile.h"
#include "LightsOut/Utility/ProcGen.h"
#include "ProceduralMeshComponent.h"
#include "Board.generated.h"

#define HEIGHT 25
#define WIDTH  25


struct FTile
{
    FProcMeshSection& MeshSection; 
    const FVector Center; 
    void(*Invocable )(AActor* Actor); 

    FTile(FProcMeshSection& meshSection, void(*temp)(AActor* Actor))
        : MeshSection(meshSection),
          Invocable(temp),
          Center(MeshSection.SectionLocalBox.GetCenter())
    {

    }

    FTile() = delete;
};

const unsigned char BoardConfiguration[WIDTH][HEIGHT] =
{
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ','#',' ',' ','#',' ',' ',' ','#',' ',' ',' ','#',' ',' ','#',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ','#',' ','#','#',' ',' ',' ','#',' ',' ',' ','#','#',' ','#',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ','#','#','#',' ',' ',' ','#','#','#',' ',' ',' ','#','#','#',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ','#',' ',' ',' ','#','#',' ',' ',' ','#','#',' ',' ',' ','#',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ','#',' ',' ',' ','#',' ',' ',' ',' ',' ','#',' ',' ',' ','#',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ','#',' ',' ','#',' ',' ','#','#','#',' ',' ','#',' ',' ','#',' ',' ',' ',' ',' '},
    {'#','#','#','#','#','#','#','#','#',' ',' ','#','#','#',' ',' ','#','#','#','#','#','#','#','#','#'},
    {' ',' ',' ',' ',' ','#',' ',' ','#',' ',' ','#','#','#',' ',' ','#',' ',' ','#',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ','#',' ',' ',' ','#',' ',' ',' ',' ',' ','#',' ',' ',' ','#',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ','#',' ',' ',' ','#','#',' ',' ',' ','#','#',' ',' ',' ','#',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ','#','#','#',' ',' ',' ','#','#','#',' ',' ',' ','#','#','#',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ','#',' ','#','#',' ',' ',' ','#',' ',' ',' ','#','#',' ','#',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ','#',' ',' ','#',' ',' ',' ','#',' ',' ',' ','#',' ',' ','#',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
};

UCLASS()
class LIGHTSOUT_API ABoard : public AActor
{
    GENERATED_BODY()

public:
    ABoard();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
    UMaterialInterface* Minoris;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
    UMaterialInterface* Majoris;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
    UMaterialInterface* Finalis;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
    UMaterialInterface* Terminus;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
    FVector SpawnLocation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
    UProceduralMeshComponent* Mesh;

    TArray<UMaterialInterface*> Textures;

    TArray<FTile> Tiles;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void CreateGrid();
};
