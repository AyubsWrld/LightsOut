// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AGrid::AGrid(const unsigned char* byteArray, size_t size)
	: Size(size)
{
	check(strlen((char*)byteArray) % Size == 0 || !byteArray);
	for (size_t i{}; i < Size; i++)
	{
		FRow Temp{};
		for (size_t j{}; j < Size; j++)
		{
			Temp.Cell.Add(byteArray[i]);
		}
		byteArray += Size; 
	}
}

// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

