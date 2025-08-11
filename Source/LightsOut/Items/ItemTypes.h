#pragma once

#include "CoreMinimal.h"
#include "ItemTypes.generated.h"


UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	IC_Undefined     UMETA(DisplayName = "Undefined"),
	IC_AudioTool     UMETA(DisplayName = "Audio Emitting Item"),
	IC_LightSource   UMETA(DisplayName = "Light Emitting Item"),
	IC_Medical       UMETA(DisplayName = "Medical Item"),
	IC_Special       UMETA(DisplayName = "Special Item")
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
};

