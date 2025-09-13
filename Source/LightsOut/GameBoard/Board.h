#pragma once

#include "CoreMinimal.h"
#include "utility"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "LightsOut/Items/ItemBase.h"
#include "array"
#include "Tile.h"
#include "LightsOut/Utility/ProcGen.h"
#include "LightsOut/Generics/Interactable.h"
#include "LightsOut/Core/BoardManager.h"
#include "ProceduralMeshComponent.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Board.generated.h"

#define HEIGHT 5
#define WIDTH  5


const unsigned char BoardConfiguration[HEIGHT][WIDTH] =
{
    {'#','#','#','#','#'},
    {'#','#','#','#','#'},
    {'#','#','#','#','#'},
    {'#','#','#','#','#'},
    {'#','#','#','#','#'}
};

/* Check size ( guessing decreasing size order right now )*/
struct FTile
{
    UStaticMeshComponent* MeshComponent{};
    FVector Center{};
    std::pair<int, int> Coordinates{};

    FTile(UStaticMeshComponent* TileMesh , std::pair<int,int> coords)
		:   MeshComponent(TileMesh),
            Coordinates(coords)
    {}
    FTile() = delete;
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
class LIGHTSOUT_API ABoard : public AActor, public IInteractable
{
    GENERATED_BODY()

private:

    TArray<FTile> StartTiles;

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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
    UStaticMesh* PlayerPieceModel;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board", Replicated)
    TArray<UStaticMeshComponent*> PlayerPieces;
    UFUNCTION(Reliable, NetMulticast)
    void MulticastMovePiece(FVector Location);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
    UStaticMesh* TileMeshAsset; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
    UMaterialInterface* DefaultTileMaterial; 

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board")
    TArray<UStaticMeshComponent*> TileMeshes;

    void SetTileColor(const std::pair<int32, int32>& Coordinates, FLinearColor NewColor);

    void SetTileMaterial(const std::pair<int32, int32>& Coordinates, UMaterialInterface* NewMaterial);

    UStaticMeshComponent* GetTileMesh(const std::pair<int32, int32>& Coordinates) const;

    TArray<UMaterialInterface*> Textures;

    TArray<FTile> Tiles;

    std::unordered_map<std::pair<int32, int32>, FTile> TileMap;

    void SpawnPlayers();

    void TestGrid();

    void DebugStartingPoints();

    std::array<std::pair<int32, int32>, 4> GetBoardBounds();

    const FVector& GetTileLocation(const std::pair<int32,int32>& Coordinates) const;

	virtual void Interact(APlayerState* Player) override ; 

protected:
    virtual void BeginPlay() override;

public:


    virtual void Tick(float DeltaTime) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void CreateGrid();
};
