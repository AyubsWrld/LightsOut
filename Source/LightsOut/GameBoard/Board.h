#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ChildActorComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Board.generated.h"

struct FPlayablableSquare
{
	UStaticMeshComponent* Square{};
	AActor* ActivePlayer{};
	void(*Invoke)(); // Interface function pointer
};

UCLASS()
class LIGHTSOUT_API ABoard : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoard();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Board")
	TArray<UStaticMeshComponent*> Squares;

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function to automatically populate the Squares array from child components
	UFUNCTION(BlueprintCallable, Category = "Board")
	void PopulateSquaresFromChildren();

	// Function to get all static mesh components from root
	UFUNCTION(BlueprintCallable, Category = "Board")
	void GetAllStaticMeshComponents();

};
