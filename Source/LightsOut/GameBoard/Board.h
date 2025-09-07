#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "array"
#include "Components/StaticMeshComponent.h"
#include "LightsOut/Items/ItemBase.h"
#include "Tile.h"
#include "LightsOut/Utility/ProcGen.h"
#include "ProceduralMeshComponent.h"
#include "Board.generated.h"

#define HEIGHT 5
#define WIDTH  5


/* Check size ( guessing decreasing size order right now )*/
struct FTile
{
    FProcMeshSection& MeshSection; 
    const FVector Center; 
    std::pair<int, int> Coordinates;
    void(*Invocable )(AActor* Actor); 

    FTile(FProcMeshSection& meshSection, void(*temp)(AActor* Actor), std::pair<short,short> coords)
        :   MeshSection(meshSection),
            Center(MeshSection.SectionLocalBox.GetCenter()),
            Coordinates(coords),
            Invocable(temp)
    {

    }

    FTile() = delete;
};

const unsigned char BoardConfiguration[WIDTH][HEIGHT] =
{
    {'#','#','#','#','#'},
    {'#','#','#','#','#'},
    {'#','#','#','#','#'},
    {'#','#','#','#','#'},
    {'#','#','#','#','#'}
};

template<>
struct std::hash<std::pair<int32,int32>>
{
    size_t operator()(const std::pair<int32,int32>& value) const noexcept
    {
        return value.first ^ value.second << 10;
    }
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

    //UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
    //FVector SpawnLocation;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
    UProceduralMeshComponent* Mesh;

    TArray<UMaterialInterface*> Textures;

    TArray<FTile> Tiles;

    std::unordered_map<std::pair<int32, int32>, FTile> TileMap;
    void SpawnPlayers();

    void TestGrid();

    void DebugStartingPoints();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void CreateGrid();
};
