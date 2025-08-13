// Fill out your copyright notice in the Description page of Project Settings.


#include "IBSingleton.h"


UIBSingleton* UIBSingleton::Main = nullptr;
void UIBSingleton::OnWorldBeginPlay(UWorld& InWorld)
{
	if (InWorld.GetNetMode() == ENetMode::NM_Client)
		return;
	SpawnItems(&InWorld);
}

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

void UIBSingleton::SpawnItems_Implementation(UWorld* World)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Spawning Items"), ANSI_TO_TCHAR(__FUNCTION__));
	if (!World) return;

	if (World->GetNetMode() == NM_Client) return; // safety

	TArray<FVector> Spawns = {
		FVector(1330.f,  920.f, 50.f),
		FVector(1330.f, 1130.f, 50.f),
		FVector(1330.f,  600.f, 50.f)
	};

	TSubclassOf<AItemBase> ItemClass = AItemBase::StaticClass();
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const FVector& Loc : Spawns)
	{
		AItemBase* Item = World->SpawnActor<AItemBase>(ItemClass, Loc, FRotator::ZeroRotator, Params);
	}
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
