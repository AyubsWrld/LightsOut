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


static TMap<ECalamity, const UMaterialInterface*> TextureMap; 
