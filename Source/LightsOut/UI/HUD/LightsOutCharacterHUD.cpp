#include "LightsOutCharacterHUD.h"
#include "Components/Image.h"


void ULightsOutCharacterHUD::SetItemSlot()
{

}

void ULightsOutCharacterHUD::RorderItems(const AItemBase& Item, int Index)
{

}

[[nodiscard]] UImage* ULightsOutCharacterHUD::GetImageAtIndex(int32 Index) const 
{
    if (!ItemSlot_0_Image && !ItemSlot_0)
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemSlot_0_Image does not exist at runtime"))
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemSlot_0_Image does indeed exist at runtime"))
    }
    switch (Index)
    {
    case 0: 
        UE_LOG(LogTemp, Warning, TEXT("Image_0"))
        return Cast<UImage>(ItemSlot_0_Image); 

    case 1: 
        UE_LOG(LogTemp, Warning, TEXT("Image_1"))
        return Cast<UImage>(ItemSlot_1_Image); 
        
    case 2: 
        UE_LOG(LogTemp, Warning, TEXT("Image_2"))
        return Cast<UImage>(ItemSlot_2_Image); 
    case 3: 
        UE_LOG(LogTemp, Warning, TEXT("Image_3"))
        return Cast<UImage>(ItemSlot_3_Image); 
    default:
        return nullptr; 
    }
}

[[nodiscard]] UWidget* ULightsOutCharacterHUD::GetSlotAtIndex(int32 Index) const 
{
    switch (Index)
    {
    case 0: 
        return ItemSlot_0;

    case 1: 
        return ItemSlot_1;
        
    case 2: 
        return ItemSlot_2;
    case 3: 
        return ItemSlot_3;
    default:
        return nullptr; 
    }
}

int32 ULightsOutCharacterHUD::GetEmptySlot() const
{
    for (int i = 0; const AItemBase* Item : Items)
    {
        if (!Item) // nullptr
            return i; 
        i++;
    }
    return -1; // Add def for -1 elsewhere. 
}

void ULightsOutCharacterHUD::RemoveAtIndex(int32 Index)
{
    if (Index >= MAX_INVENTORY_SIZE || Index < 0)
        return; 

    Items[Index] = nullptr; 
}

void ULightsOutCharacterHUD::DebugItemSlot(int32 Index)
{
    UE_LOG(LogTemp, Warning, TEXT("Setting item slot"));
    UWidget* TmpSlot = GetSlotAtIndex(Index);
    if (!TmpSlot)
    {
		UE_LOG(LogTemp, Warning, TEXT("TMP_UNDEF"));
        return; 
    }
    if (CurrentActive)
        CurrentActive->SetRenderOpacity(1.0f);
    CurrentActive = TmpSlot;
    TmpSlot->SetRenderOpacity(0.5f);
}


void ULightsOutCharacterHUD::UpdateHUD(AItemBase* Item)
{
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s]: Invalid ptr to item"), ANSI_TO_TCHAR(__FUNCTION__));
        return;
    }

	UE_LOG(LogTemp, Warning, TEXT("[%s]: Valid ptr to item"), ANSI_TO_TCHAR(__FUNCTION__));

    switch( Item->GetType())
    {
    case EItemCategory::IC_Undefined: 
        UE_LOG(LogTemp, Warning, TEXT("[%s]: Undefined"), ANSI_TO_TCHAR(__FUNCTION__));

    case EItemCategory::IC_LightSource:
        UE_LOG(LogTemp, Warning, TEXT("[%s]: LightSource"), ANSI_TO_TCHAR(__FUNCTION__));
    }

    int32 Index = GetEmptySlot();
    if (Index != -1)
    {
        UE_LOG(LogTemp, Warning, TEXT("Checking at idx: %d"), Index);
        UImage* ItemImage = GetImageAtIndex(Index);
        if (!ItemImage)
        {
            UE_LOG(LogTemp, Warning, TEXT("No Image"));
            return;
        }

        // Use default white texture from engine resources

        UTexture2D* Icon = Item->GetIcon();
        if (!Icon)
        {
            UE_LOG(LogTemp, Warning, TEXT("Item Icon Undef"));
            ItemImage->SetBrushFromTexture(DefaultIcon, true);
        }
        else
        {
            ItemImage->SetBrushFromTexture(Icon, true);
        }

		Items[Index] = Item; 
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("No Empty Slot"));
}


[[nodiscard]] bool ULightsOutCharacterHUD::IsWellFormed() const
{
    TArray<FName> Names;
    GetSlotNames(Names);
    for (FName n : Names)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *n.ToString());
    }
    return true;
}

ULightsOutCharacterHUD::ULightsOutCharacterHUD(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void ULightsOutCharacterHUD::NativeConstruct()
{
    Super::NativeConstruct();

    if (!ItemSlot_0_Image || !ItemSlot_0)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemSlot_0 widgets are not properly bound! Check Blueprint widget names."));
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("ItemSlot_0 widgets successfully bound."));
    }

    UE_LOG(LogTemp, Log, TEXT("Widget binding status:"));
    UE_LOG(LogTemp, Log, TEXT("ItemSlot_0_Image: %s"), ItemSlot_0_Image ? TEXT("Valid") : TEXT("NULL"));
    UE_LOG(LogTemp, Log, TEXT("ItemSlot_1_Image: %s"), ItemSlot_1_Image ? TEXT("Valid") : TEXT("NULL"));
    UE_LOG(LogTemp, Log, TEXT("ItemSlot_2_Image: %s"), ItemSlot_2_Image ? TEXT("Valid") : TEXT("NULL"));
    UE_LOG(LogTemp, Log, TEXT("ItemSlot_3_Image: %s"), ItemSlot_3_Image ? TEXT("Valid") : TEXT("NULL"));
    UE_LOG(LogTemp, Log, TEXT("ItemSlot_0: %s"), ItemSlot_0 ? TEXT("Valid") : TEXT("NULL"));
    UE_LOG(LogTemp, Log, TEXT("ItemSlot_1: %s"), ItemSlot_1 ? TEXT("Valid") : TEXT("NULL"));
    UE_LOG(LogTemp, Log, TEXT("ItemSlot_2: %s"), ItemSlot_2 ? TEXT("Valid") : TEXT("NULL"));
    UE_LOG(LogTemp, Log, TEXT("ItemSlot_3: %s"), ItemSlot_3 ? TEXT("Valid") : TEXT("NULL"));
}
