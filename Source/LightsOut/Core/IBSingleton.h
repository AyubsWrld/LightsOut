// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "LightsOut/Generics/ItemBroker.h"
#include "LightsOut/Items/LightSource.h"
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/Engine.h"
#include "type_traits"
#include "IBSingleton.generated.h"

/**
 * 
 */

/* Functor for generating Items */

struct FSpawnLocations
{
	TArray<FVector> SpawnLocations
	{
		FVector(1330.f,  920.f, 50.f),
		FVector(1330.f, 1130.f, 50.f),
		FVector(1330.f,  600.f, 50.f)
	};

	const TArray<FVector>& Get() const
	{ 
		return SpawnLocations; 
	}
};

struct FItemGenerator
{
	static constexpr auto ItemCategoryMax{ static_cast<std::underlying_type_t<EItemCategory>>(EItemCategory::IC_MAX_BOUND)};

	FSpawnLocations SpawnLocations{};
	AItemBase* SpawnItem(EItemCategory ItemType, UWorld& World, const FVector& Location)
	{
		//static constexpr auto ItemCategoryMax{ static_cast<std::underlying_type_t<EItemCategory>>(EItemCategory::IC_MAX_BOUND)};
		switch (ItemType)
		{
		case EItemCategory::IC_Undefined :
			UE_LOG(LogTemp, Warning, TEXT("[%s]: Spawning Undefined"), ANSI_TO_TCHAR(__FUNCTION__));
			return World.SpawnActor<AItemBase>(Location, FRotator::ZeroRotator); 
		case EItemCategory::IC_AudioTool :
			UE_LOG(LogTemp, Warning, TEXT("[%s]: Generating AudioTool"), ANSI_TO_TCHAR(__FUNCTION__));
			return World.SpawnActor<AItemBase>(Location, FRotator::ZeroRotator); 
		case EItemCategory::IC_LightSource:
			UE_LOG(LogTemp, Warning, TEXT("[%s]: Generating LightSource"), ANSI_TO_TCHAR(__FUNCTION__));
			return World.SpawnActor<AItemBase>(Location, FRotator::ZeroRotator); 
		case EItemCategory::IC_Medical :
			UE_LOG(LogTemp, Warning, TEXT("[%s]: Generating Medical"), ANSI_TO_TCHAR(__FUNCTION__));
			return World.SpawnActor<AItemBase>(Location, FRotator::ZeroRotator); 
		case EItemCategory::IC_Special :
			UE_LOG(LogTemp, Warning, TEXT("[%s]: Generating Special"), ANSI_TO_TCHAR(__FUNCTION__));
			return World.SpawnActor<AItemBase>(Location, FRotator::ZeroRotator); 
		default:
			UE_LOG(LogTemp, Warning, TEXT("[%s]: Generating Undefined"), ANSI_TO_TCHAR(__FUNCTION__));
			return World.SpawnActor<AItemBase>(Location, FRotator::ZeroRotator); 
		}
	}
	void SpawnItems(UWorld* World, TArray<AItemBase*>& out)
	{
		for (int i{}; const auto& loc : SpawnLocations.Get())
		{
			out.Add(SpawnItem(static_cast<EItemCategory>(rand() % ItemCategoryMax), *World, loc));
		}
	}
};

UCLASS()
class LIGHTSOUT_API UIBSingleton : public UWorldSubsystem, public IItemBroker
{
	GENERATED_BODY()

public:

	static UIBSingleton* Main;

	FItemGenerator   ItemGenerator; 
	InventoryMap     PlayerInventories;
	ItemRegistry     Registry;
	ItemSpawnPoints  SpawnPoints;

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

