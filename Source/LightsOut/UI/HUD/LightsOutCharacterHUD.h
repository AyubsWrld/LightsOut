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
UCLASS(Abstract)
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
	UNamedSlot* ItemSlot_0;
	UPROPERTY(meta = (BindWidget))
	UImage* ItemSlot_0_Image;
	UPROPERTY(meta = (BindWidget))
	UNamedSlot* ItemSlot_1;
	UPROPERTY(meta = (BindWidget))
	UImage* ItemSlot_1_Image;
	UPROPERTY(meta = (BindWidget))
	UNamedSlot* ItemSlot_2;
	UPROPERTY(meta = (BindWidget))
	UImage* ItemSlot_2_Image;
	UPROPERTY(meta = (BindWidget))
	UNamedSlot* ItemSlot_3;
	UPROPERTY(meta = (BindWidget))
	UImage* ItemSlot_3_Image;

protected:
	// Called when the widget is constructed
	virtual void NativeConstruct() override;

private:
	// Helper function to get image widget by index
	UImage* GetImageWidgetByIndex(int32 SlotIndex) const;

public:
	/* Wrapper around SetContentForSlot */
	void SetItemSlot();
	void RorderItems(const AItemBase& Item, int Index);
	bool [[nodiscard]] IsWellFormed() const; // Invariance check; 

	// Function to set image brush for a specific slot
	void SetSlotImage(int32 SlotIndex, UTexture2D* ItemTexture);

	// Function to clear a slot image
	void ClearSlotImage(int32 SlotIndex);

	// Function to set image using item data
	void SetSlotImageFromItem(int32 SlotIndex, const AItemBase* Item);

	// Set all slots to a default color
	void SetAllSlotsToDefaultColor(FLinearColor Color = FLinearColor::Gray);

	// Set a specific slot to a color
	void SetSlotToColor(int32 SlotIndex, FLinearColor Color);

	// Set all slots to transparent/hidden
	void ClearAllSlots();

	// Highlight/unhighlight a specific slot
	void SetHighlightSlot(int32 SlotIndex, bool bHighlight);
};
