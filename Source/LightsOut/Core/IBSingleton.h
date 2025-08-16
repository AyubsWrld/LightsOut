// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LightsOut/Generics/ItemBroker.h"
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/Engine.h"
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

	InventoryMap     PlayerInventories;
	ItemRegistry    Registry;
	ItemSpawnPoints SpawnPoints;

	UIBSingleton(); 

	// Causes C2535: https://learn.microsoft.com/en-us/cpp/error-messages/compiler-errors-2/compiler-error-c2535?view=msvc-170. 
	/*
	UIBSingleton(const UIBSingleton& Other) = delete;
	UIBSingleton& operator=(const UIBSingleton&) = delete;
	*/

	/*static [[nodiscard]] UIBSingleton* Get();*/

	/*
	template<typename T>
	static [[nodiscard]] UIBSingleton* GetFrom(T* WorldObjectContext);
	*/

	void OnWorldBeginPlay(UWorld& InWorld) override;

	[[nodiscard]] virtual bool TryAddToPlayerInventory(AItemBase& Item, PID PlayerID) override;

	[[nodiscard]] virtual bool PlayerOwnsItem(AItemBase& Item, PID PlayerID) const override;

	virtual void RemoveFromPlayerInventory(AItemBase& Item, PID PlayerID) override;

	virtual void DeletePlayerInventory(PID PlayerID) override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastSpawnItems(UWorld* World) override;

	virtual ItemSpawnPoints* GetItemSpawns() const override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

};

