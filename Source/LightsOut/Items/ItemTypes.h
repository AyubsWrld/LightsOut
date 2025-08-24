#pragma once

#include "CoreMinimal.h"
#include "LightsOut/Items/ItemBase.h"
#include "ItemTypes.generated.h"

typedef       FGuid                               IID; 
typedef       FGuid                               PID; 
typedef       TMap<PID, TArray<AItemBase*>>       InventoryMap; 
//typedef       TMap<IID, AItemBase*>               ItemRegistry; 
typedef       TArray<AItemBase*>       ItemRegistry; 
typedef       TArray<FVector>                     ItemSpawnPoints; 

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	IC_Undefined     UMETA(DisplayName = "Undefined"),
	IC_AudioTool     UMETA(DisplayName = "Audio Emitting Item"),
	IC_LightSource   UMETA(DisplayName = "Light Emitting Item"),
	IC_Medical       UMETA(DisplayName = "Medical Item"),
	IC_Special       UMETA(DisplayName = "Special Item"),
	IC_MAX_BOUND     UMETA(DisplayName = "Max Bound") // Not for use. but for bound checking when generating items randomly. 
};


USTRUCT(BlueprintType)
struct FItemSlot
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid IID;

	UPROPERTY()
	FString Icon;

	FItemSlot() = default;

	FItemSlot(FGuid id, FString iconURL)
	{
		/* ... Check if string is valid ... */

		IID = id; 
		Icon = iconURL; 

	}

};

UENUM(BlueprintType)
enum class EItemSpawnConfig : uint8
{
	ISC_Default, 
	ISC_Assorted
};
