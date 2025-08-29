// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "Materials/MaterialInterface.h"

UENUM()
enum class ECalamity
{
	EC_Minoris UMETA(DisplayName = "Minoris"),
	EC_Majoris UMETA(DisplayName = "Majoris"),
	EC_Finalis UMETA(DisplayName = "Finalis"),
	EC_Terminus UMETA(DisplayName = "Terminus"),
	EC_Max UMETA(DisplayName = "Max Value")
};

/* Might be an issue. perhaps const cast? */
/* TODO: Populate in BP_Editor */
namespace CConstants
{
	static TMap<ECalamity, const UMaterialInterface*> TextureMap;
}


