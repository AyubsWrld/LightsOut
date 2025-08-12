// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LightsOut/Items/ItemTypes.h"
#include "ItemBroker.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemBroker : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LIGHTSOUT_API IItemBroker
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:

	virtual void AddToPlayerInventory(AItemBase& Item, PID PlayerID);
	virtual void RemoveFromPlayerInventory(AItemBase& Item, PID PlayerID);
	virtual void DeletePlayerInventory(PID PlayerID);

	virtual void SpawnItems(UWorld* World);
	virtual [[nodiscard]] bool PlayerOwnsItem(AItemBase& Item, PID PlayerID) const ;
	/* virtual [[nodiscard]] const AItemBase& InspectItem(IID ItemID); */
	/* Convert to reference later */
	virtual ItemSpawnPoints* GetItemSpawns() const ;
};
