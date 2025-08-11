// Fill out your copyright notice in the Description page of Project Settings.

#include "IBSingleton.h"


IBSingleton* IBSingleton::Main = nullptr;

IBSingleton* IBSingleton::Get()
{
	if (IBSingleton::Main == nullptr)
	{
		IBSingleton::Main = new IBSingleton();
	}
	return IBSingleton::Main; 
}
void IBSingleton::AddToPlayerInventory(AItemBase& Item, PID PlayerID)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Adding (IID: %s) to Player(%s) Inventory"), ANSI_TO_TCHAR(__FUNCTION__), *Item.GetID().ToString(), *PlayerID.ToString());
}

void IBSingleton::RemoveFromPlayerInventory(AItemBase& Item, PID PlayerID)
{
	UE_LOG(LogTemp, Warning, TEXT("Removing from player inventory"));
}

void IBSingleton::DeletePlayerInventory(PID PlayerID)
{
	UE_LOG(LogTemp, Warning, TEXT("Deleting from player inventory"));
}

bool IBSingleton::PlayerOwnsItem(AItemBase& Item, PID PlayerID) const
{
	return false;
}

