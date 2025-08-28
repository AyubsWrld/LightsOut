#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Engine/StaticMeshActor.h"
#include "LightsOut/Core/Calamities/CalamityInfo.h"
#include "Board.generated.h"


struct FTile
{
	UStaticMeshComponent* Mesh{};

	const AActor* ActivePlayer{};

	void(*Invoke)() {};

	ECalamity Calamity{}; // Defaults to EC_Calamity::Minoris

	inline void SetActivePlayer(AActor& Actor) {};


	FTile(UStaticMeshComponent& Tile, void(*cb)()) :
		Mesh(&Tile),
		Invoke(cb)
	{
		if (Invoke)
			Invoke();
	}

	FTile(UStaticMeshComponent& Tile) :
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

	TArray<FTile> Tiles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
	UMaterialInterface* Minoris;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
	UMaterialInterface* Majoris;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
	UMaterialInterface* Finalis;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Board")
	UMaterialInterface* Terminus;

	TArray<UMaterialInterface*> Textures = { Minoris, Majoris, Finalis, Terminus }; 

	UFUNCTION()
	void UpdateSquares();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Board")
	void PopulateSquaresFromChildren();

};
