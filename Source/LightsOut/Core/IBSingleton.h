// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LightsOut/Generics/ItemBroker.h"
#include "LightsOut/Items/ItemBase.h"
#include "CoreMinimal.h"

/**
 * 
 */
class LIGHTSOUT_API IBSingleton : public IItemBroker
{

public:

	static IBSingleton* Main ; 

	InvetoryMap PlayerInventoreies;
	IBSingleton() = default;

	IBSingleton(IBSingleton& Other) = delete; 

	void operator=(const IBSingleton&) = delete;


	static [[nodiscard]] IBSingleton* Get();

	virtual void AddToPlayerInventory(AItemBase& Item, PID PlayerID) override ;
	virtual void RemoveFromPlayerInventory(AItemBase& Item, PID PlayerID) override;
	virtual void DeletePlayerInventory(PID PlayerID) override ;
	virtual [[nodiscard]] bool PlayerOwnsItem(AItemBase& Item, PID PlayerID) const override;
};
