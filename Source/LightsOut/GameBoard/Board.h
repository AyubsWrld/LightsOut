#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Engine/StaticMeshActor.h"
#include "LightsOut/Items/ItemBase.h"
#include "LightsOut/Core/Calamities/CalamityInfo.h"
#include "Engine/StaticMeshActor.h"
#include "Board.generated.h"


struct FTile
{
	AStaticMeshActor* Mesh{};

	AActor* ActivePlayer{};

	FVector Center{}; 

	ECalamity Calamity{}; 

	FTile(AStaticMeshActor& Tile) :
		Mesh(&Tile)
	{
	}
};

UCLASS()
class LIGHTSOUT_API ABoard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
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
	AActor* PlayerPiece;

	TArray<UMaterialInterface*> Textures = { Minoris, Majoris, Finalis, Terminus }; 

	TArray<FTile> Tiles;

	TArray<FTile> StartTiles;

	UFUNCTION()
	void SetStartingPosition(int32 PlayerCount);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Board")
	void PopulateSquaresFromChildren();

};
