// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

// Sets default values
AItemBase::AItemBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// Assign Mesh

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirstPersonCamera"));

	// Assign IID

	IID = FGuid::NewGuid();

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("It Worked")); 
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
[[nodiscard]] FGuid AItemBase::GetID() const
{
	return this->IID; 
}
void AItemBase::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("[Interaction] Item Interaction Invoked"));
}

