// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LightsOut/Generics/ItemBroker.h"
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "IBSingleton.generated.h"

/**
 * 
 */

UCLASS()
class LIGHTSOUT_API UIBSingleton : public UWorldSubsystem, public IItemBroker
{
	GENERATED_BODY()

public:

	static UIBSingleton* Main;

	InvetoryMap     PlayerInventoreies;
	ItemRegistry    Registry;
	ItemSpawnPoints SpawnPoints;

	UIBSingleton(); 

	// Causes C2535: https://learn.microsoft.com/en-us/cpp/error-messages/compiler-errors-2/compiler-error-c2535?view=msvc-170. 
	/*
	UIBSingleton(const UIBSingleton& Other) = delete;
	UIBSingleton& operator=(const UIBSingleton&) = delete;
	*/

	static [[nodiscard]] UIBSingleton* Get(UWorld* World);

	/*
	template<typename T>
	static [[nodiscard]] UIBSingleton* GetFrom(T* WorldObjectContext);
	*/

	virtual void AddToPlayerInventory(AItemBase& Item, PID PlayerID) override;
	virtual void RemoveFromPlayerInventory(AItemBase& Item, PID PlayerID) override;
	virtual void DeletePlayerInventory(PID PlayerID) override;
	virtual [[nodiscard]] bool PlayerOwnsItem(AItemBase& Item, PID PlayerID) const override;

	virtual void SpawnItems(UWorld* World) override;
	virtual void SetItemSpawns(ItemSpawnPoints& spawnpoints) override;
	virtual ItemSpawnPoints* GetItemSpawns() const override;

};

