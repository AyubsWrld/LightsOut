// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBroker.h"

// Add default functionality here for any IItemBroker functions that are not pure virtual.

void IItemBroker::AddToPlayerInventory(AItemBase& Item, PID PlayerID)
{
	UE_LOG(LogTemp, Warning, TEXT("Invoked from generic interface"));
}

void IItemBroker::RemoveFromPlayerInventory(AItemBase& Item , PID PlayerID)
{
	UE_LOG(LogTemp, Warning, TEXT("Invoked from generic interface"));
}

void IItemBroker::DeletePlayerInventory(PID PlayerID)
{
	UE_LOG(LogTemp, Warning, TEXT("Invoked from generic interface"));
}

void IItemBroker::MulticastSpawnItems(UWorld* World)
{
	UE_LOG(LogTemp, Warning, TEXT("Invoked from generic interface"));
}

bool IItemBroker::PlayerOwnsItem(AItemBase& Item, PID PlayerID) const
{
	UE_LOG(LogTemp, Warning, TEXT("Invoked from generic interface"));
	return false;
}

ItemSpawnPoints* IItemBroker::GetItemSpawns() const
{
	return nullptr;
}

