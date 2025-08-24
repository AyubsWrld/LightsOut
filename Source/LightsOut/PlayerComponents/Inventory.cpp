// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#define MAX_ITEMS 4 // Just a tad bit superflous 

// Sets default values for this component's properties
UInventory::UInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventory::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventory::AddItem(const AItemBase* Item)
{
	/*
	if (Items.Num() < MAX_ITEMS)
		return; 
	Items.Add(MakeShared<AItemBase>(&Item));
	*/
}

void UInventory::EquipItem(int32 Index)
{
}

AItemBase* UInventory::GetItemAtIndex(int32 Index) const
{
	if (Items.IsValidIndex(Index))
	{
		if (AItemBase* Temp{Items[Index]}; Temp)
			return Temp; 
	}
	return{};
}

void UInventory::DiscardItem(int32 Index)
{

}
