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

	[[nodiscard]] virtual bool TryAddToPlayerInventory(AItemBase& Item, PID PlayerID);

	[[nodiscard]] virtual bool PlayerOwnsItem(AItemBase& Item, PID PlayerID) const ;
	virtual void RemoveFromPlayerInventory(AItemBase& Item, PID PlayerID);
	virtual void DeletePlayerInventory(PID PlayerID);

	virtual void MulticastSpawnItems(UWorld* World);
	/* virtual [[nodiscard]] const AItemBase& InspectItem(IID ItemID); */
	/* Convert to reference later */
	virtual ItemSpawnPoints* GetItemSpawns() const ;
};
