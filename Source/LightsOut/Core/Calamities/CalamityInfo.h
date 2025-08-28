// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "Materials/MaterialInterface.h"

enum class ECalamity
{
	EC_Minoris,
	EC_Majoris,
	EC_Finalis,
	EC_Terminus,
	EC_Max
};

/* Might be an issue. perhaps const cast? */
/* TODO: Populate in BP_Editor */
namespace CConstants
{
	static TMap<ECalamity, const UMaterialInterface*> TextureMap;
}


