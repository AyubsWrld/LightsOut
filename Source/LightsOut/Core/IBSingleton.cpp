// Fill out your copyright notice in the Description page of Project Settings.

#include "IBSingleton.h"


UIBSingleton* UIBSingleton::Main = nullptr;



UIBSingleton* UIBSingleton::Get(UWorld* World)
{
	if (!World)
		return nullptr;
	return World->GetSubsystem<UIBSingleton>();
}

/*
template<typename T> 
UIBSingleton* UIBSingleton::GetFrom(T* WorldObjectContext)
{
	if (!WorldContextObject) { return nullptr; }
	if (UWorld* World = WorldContextObject->GetWorld())
	{
		return World->GetSubsystem<UIBSingleton>();
	}
	return nullptr;
}
*/

void UIBSingleton::AddToPlayerInventory(AItemBase& Item, PID PlayerID)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Adding (IID: %s) to Player(%s) Inventory"), ANSI_TO_TCHAR(__FUNCTION__), *Item.GetID().ToString(), *PlayerID.ToString());
}

void UIBSingleton::RemoveFromPlayerInventory(AItemBase& Item, PID PlayerID)
{
	UE_LOG(LogTemp, Warning, TEXT("Removing from player inventory"));
}

void UIBSingleton::DeletePlayerInventory(PID PlayerID)
{
	UE_LOG(LogTemp, Warning, TEXT("Deleting from player inventory"));
}

bool UIBSingleton::PlayerOwnsItem(AItemBase& Item, PID PlayerID) const
{
	return false;
}

void UIBSingleton::SpawnItems(UWorld* World)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Spawning Items"), ANSI_TO_TCHAR(__FUNCTION__));
}

void UIBSingleton::SetItemSpawns(ItemSpawnPoints& spawnpoints)
{
	UIBSingleton* instance = UIBSingleton::Get(GetWorld()); 
	instance->SpawnPoints = spawnpoints;
}

ItemSpawnPoints* UIBSingleton::GetItemSpawns() const
{
	return nullptr;
}

UIBSingleton::UIBSingleton()
{
	ItemSpawnPoints Spawns = {
		FVector(1330.0f, 920.0f, 0.0),
		FVector(1330.0f, 1130.0f, 0.0),
		FVector(1330.0f, 600.0f, 0.0)
	};
}
