// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid.generated.h"

/* nxn grid */

USTRUCT()
struct FRow
{
	GENERATED_BODY()
	TArray<char> Cell;
};

UCLASS()
class LIGHTSOUT_API AGrid : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AGrid();

	AGrid(const unsigned char* ByteArray, size_t Size);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cell")
	AActor* Cell; 

	size_t Size; 

	TArray<FRow> Grid;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
