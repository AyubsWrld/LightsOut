// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LightsOut/Items/ItemTypes.h"
#include "LightsOut/Items/ItemBase.h"
#include "Components/NamedSlot.h"
#include "Styling/SlateBrush.h"
#include "Components/Image.h"
#include "LightsOutCharacterHUD.generated.h"

#define MAX_NUM_SLOTS 4 

/**
 *
 */
UCLASS()
class LIGHTSOUT_API ULightsOutCharacterHUD : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FItemSlot> ItemSlots;

	const FName SlotNames[4] =
	{
		TEXT("ItemSlot_0"),
		TEXT("ItemSlot_1"),
		TEXT("ItemSlot_2"),
		TEXT("ItemSlot_3")
	};

	UPROPERTY(meta = (BindWidget))
	UWidget* ItemSlot_0;

	UPROPERTY(meta = (BindWidget))
	UWidget* ItemSlot_0_Image;

	UPROPERTY(meta = (BindWidget))
	UWidget* ItemSlot_1;

	UPROPERTY(meta = (BindWidget))
	UWidget* ItemSlot_1_Image;

	UPROPERTY(meta = (BindWidget))
	UWidget* ItemSlot_2;

	UPROPERTY(meta = (BindWidget))
	UWidget* ItemSlot_2_Image;

	UPROPERTY(meta = (BindWidget))
	UWidget* ItemSlot_3;

	UPROPERTY(meta = (BindWidget))
	UWidget* ItemSlot_3_Image;

	UPROPERTY(meta = (BindWidget))
	UWidget* RHorizontalBox;

	UPROPERTY(meta = (BindWidget))
	UWidget* MainPanel;

protected:

private:

public:
	/* Wrapper around SetContentForSlot */
	void SetItemSlot();
	void RorderItems(const AItemBase& Item, int Index);
	bool [[nodiscard]] IsWellFormed() const; // Invariance check; 

};
