#include "LightsOutCharacterHUD.h"
#include "Components/Image.h"
#include "Engine/Engine.h"

void ULightsOutCharacterHUD::NativeConstruct()
{
    Super::NativeConstruct();

    // Set default color when widget is created
    SetAllSlotsToDefaultColor(FLinearColor(0.1f, 0.1f, 0.1f, 0.8f));
}

void ULightsOutCharacterHUD::SetItemSlot()
{
    for (int32 i = 0; i < MAX_NUM_SLOTS; i++)
    {

		SetSlotToColor(i, FLinearColor(0.2f, 0.2f, 0.2f, 0.5f)); // Dark gray, semi-transparent

        /*
        if (i < ItemSlots.Num() && ItemSlots[i].Item.IsValid() && false)
        {
            // Set the image from the item
            //SetSlotImageFromItem(i, ItemSlots[i].Item.Get());
        }
        else
        {
            // Set to default empty slot color instead of clearing
        }
        */
    }
}

void ULightsOutCharacterHUD::RorderItems(const AItemBase& Item, int Index)
{
    // Implementation for reordering items
    if (Index < 0 || Index >= MAX_NUM_SLOTS)
    {
        return;
    }

    // Add your reordering logic here
    // This is just a placeholder implementation
}

void ULightsOutCharacterHUD::SetAllSlotsToDefaultColor(FLinearColor Color)
{
    for (int32 i = 0; i < MAX_NUM_SLOTS; i++)
    {
        SetSlotToColor(i, Color);
    }
}

void ULightsOutCharacterHUD::SetSlotToColor(int32 SlotIndex, FLinearColor Color)
{
    if (SlotIndex < 0 || SlotIndex >= MAX_NUM_SLOTS)
    {
        return;
    }

    UImage* ImageWidget = GetImageWidgetByIndex(SlotIndex);
    if (!ImageWidget)
    {
        return;
    }

    ImageWidget->ColorAndOpacity = Color;
}

void ULightsOutCharacterHUD::SetSlotImage(int32 SlotIndex, UTexture2D* ItemTexture)
{
    // Validate slot index
    if (SlotIndex < 0 || SlotIndex >= MAX_NUM_SLOTS)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid slot index: %d"), SlotIndex);
        return;
    }

    UImage* ImageWidget = GetImageWidgetByIndex(SlotIndex);
    if (!ImageWidget)
    {
        UE_LOG(LogTemp, Warning, TEXT("Image widget not found for slot %d"), SlotIndex);
        return;
    }

    if (ItemTexture)
    {
        /*
        // Create a new brush with the texture
        FSlateBrush NewBrush;
        NewBrush.SetResourceObject(ItemTexture);
        NewBrush.ImageSize = FVector2D(64.0f, 64.0f); // Adjust size as needed
        NewBrush.DrawAs = ESlateBrushDrawType::Image;

        ImageWidget->SetBrush(NewBrush);
        ImageWidget->SetVisibility(ESlateVisibility::Visible);
        */
    }
    else
    {
        // Clear the image if no texture provided
        //ClearSlotImage(SlotIndex);
    }
}

void ULightsOutCharacterHUD::ClearSlotImage(int32 SlotIndex)
{
}

void ULightsOutCharacterHUD::SetSlotImageFromItem(int32 SlotIndex, const AItemBase* Item)
{
    if (!Item)
    {
        return;
    }

}

void ULightsOutCharacterHUD::ClearAllSlots()
{
}

void ULightsOutCharacterHUD::SetHighlightSlot(int32 SlotIndex, bool bHighlight)
{
}

UImage* ULightsOutCharacterHUD::GetImageWidgetByIndex(int32 SlotIndex) const
{
    switch (SlotIndex)
    {
    case 0: return ItemSlot_0_Image;
    case 1: return ItemSlot_1_Image;
    case 2: return ItemSlot_2_Image;
    case 3: return ItemSlot_3_Image;
    default: return nullptr;
    }
}

bool ULightsOutCharacterHUD::IsWellFormed() const
{
    TArray<FName> Names;
    GetSlotNames(Names);
    for (FName n : Names)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *n.ToString());
    }
    return true;
}
