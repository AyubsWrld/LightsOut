// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSource.h"
#include "LightsOut/Items/ItemTypes.h"


FString ALightSource::GetThumbnailURI() const
{
	return TEXT("DEFAULTURI");
}

UTexture2D* ALightSource::GetIcon() const
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: Getting Texture"), ANSI_TO_TCHAR(__FUNCTION__));
	if (!this->Icon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Icon Undefined"));
	}
	return this->Icon;
}

FGuid ALightSource::GetID() const
{
	return GetActorGuid(); // Consistent Across games? 
}


void ALightSource::Use()
{
	UE_LOG(LogTemp, Warning, TEXT("Using Lightsource"));
}

void ALightSource::Equip()
{

	UE_LOG(LogTemp, Warning, TEXT("Equipping lightsource"));
}

void ALightSource::Drop(const FVector&& Location)
{
	UE_LOG(LogTemp, Warning, TEXT("Dropping LightSource at: (%f, %f, %f)"), Location.X, Location.Y, Location.Z);
}

EItemCategory ALightSource::GetType() const
{
	return EItemCategory::IC_LightSource; 
}

