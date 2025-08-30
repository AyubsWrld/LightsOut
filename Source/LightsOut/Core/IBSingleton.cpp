// Fill out your copyright notice in the Description page of Project Settings.


#include "IBSingleton.h"


UIBSingleton* UIBSingleton::Main = nullptr;
void DespawnItem(AActor& Item)
{
	Item.SetActorHiddenInGame(true);
	Item.SetActorEnableCollision(false);
	Item.SetActorTickEnabled(false);
}

void UIBSingleton::OnWorldBeginPlay(UWorld& InWorld)
{
	if (InWorld.GetNetMode() == ENetMode::NM_Client)
		return;
	MulticastSpawnItems(&InWorld);
}

bool UIBSingleton::TryAddToPlayerInventory(AItemBase& Item, PID PlayerID)
{
	if (GetWorld()->GetNetMode() != ENetMode::NM_DedicatedServer) // sanity
		return false; 

	/* Create the player a new inventory, Should this be done when they attempt to pick up their first item?*/
	if (!PlayerInventories.Contains(PlayerID))
		PlayerInventories.Add(PlayerID, TArray<AItemBase*>());

	TArray<AItemBase*>& Inventory = PlayerInventories[PlayerID];
	Inventory.AddUnique(&Item); // Const lvalue reference to 

	for (auto Item : PlayerInventories[PlayerID])
		UE_LOG(LogTemp, Warning, TEXT("[%s]: %s"), ANSI_TO_TCHAR(__FUNCTION__), *Item->GetActorGuid().ToString());
	DespawnItem(Item);
	return true; 
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

/* Don't need to pass in pointer */
void UIBSingleton::MulticastSpawnItems_Implementation(UWorld* World)
{
	if (!World) return;

	if (World->GetNetMode() == NM_Client) return; // safety
	ItemGenerator.SpawnItems(GetWorld(), Registry);
}

ItemSpawnPoints* UIBSingleton::GetItemSpawns() const
{
	return nullptr;
}

bool UIBSingleton::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	if (!World || !World->IsGameWorld()) return false;

	const ENetMode NM = World->GetNetMode();
	return (NM == NM_DedicatedServer || NM == NM_ListenServer);
}

UIBSingleton::UIBSingleton()
{
}

