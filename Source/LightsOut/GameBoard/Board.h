#pragma once

#include "CoreMinimal.h"
#include "utility"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "LightsOut/Items/ItemBase.h"
#include "array"
#include "Tile.h"
#include "LightsOut/Utility/ProcGen.h"
#include "LightsOut/Generics/Interactable.h"
#include "LightsOut/LightsOutCharacter.h"
#include "ProceduralMeshComponent.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Board.generated.h"

#define HEIGHT 5
#define WIDTH  5

class UBoardManager;

struct FTile
{
    using FCartesianCoordinates = std::pair<int32, int32>;

    UStaticMeshComponent*       MeshComponent{}        ;
    FVector                     Center{}               ;
    FCartesianCoordinates       Coordinates{}          ;

    FTile(UStaticMeshComponent* TileMesh , FCartesianCoordinates coords)
		:   MeshComponent(TileMesh),
            Coordinates(coords)
    {}
    FTile() = delete;
};

struct FPlayerPieceMetadata
{
    using FCartesianCoordinates = std::pair<int32, int32>;

    FCartesianCoordinates       Coordinates{}          ;
    ALightsOutCharacter*        Owner{}                ;
    FVector                     Location{}             ;
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

    UFUNCTION()
    void HandleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
    UStaticMesh* TileMeshAsset; 

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Board")
    UMaterialInterface* DefaultTileMaterial; 

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Board")
    TArray<UStaticMeshComponent*> TileMeshes;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    UBoxComponent* BoxCollider;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    TArray<TObjectPtr<ALightsOutCharacter>> PlayersInColliderVolume;

    UFUNCTION()
	void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



    void SetTileColor(const std::pair<int32, int32>& Coordinates, FLinearColor NewColor);
    void SetTileMaterial(const std::pair<int32, int32>& Coordinates, UMaterialInterface* NewMaterial);
    bool IsViewInterest();
    void SpawnPlayers();

    void SetStartingPoints();

    std::array<std::pair<int32, int32>, 4> GetBoardBounds();

    const FVector& GetTileLocation(const std::pair<int32,int32>& Coordinates) const;

    void Highlight(); 

	virtual void Interact(APlayerState* Player) override ; 

    UStaticMeshComponent* GetTileMesh(const std::pair<int32, int32>& Coordinates) const;

    TArray<UMaterialInterface*> Textures;

    TArray<FTile> Tiles;

    std::unordered_map<std::pair<int32, int32>, FTile> TileMap;

    std::unordered_map<UStaticMeshComponent*, FPlayerPieceMetadata> PlayerPiecesMetadata;
    
    bool bQueryIsInterest;

    friend class UBoardmanager;

protected:
    virtual void BeginPlay() override;

public:

    virtual void Tick(float DeltaTime) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void CreateGrid();
};
