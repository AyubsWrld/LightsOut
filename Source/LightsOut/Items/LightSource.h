// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LightsOut/Items/ItemBase.h"
#include "LightSource.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOUT_API ALightSource : public AItemBase
{
	GENERATED_BODY()

	virtual void Use() override;

	virtual void Equip() override ;

	virtual void Drop(const FVector&& Location) override;

 	[[nodiscard]] virtual FGuid GetID() const override;

	[[nodiscard]] virtual FString GetThumbnailURI() const override;

	[[nodiscard]] virtual UTexture2D* GetIcon() const override ; 


	virtual EItemCategory GetType() const override;


};
