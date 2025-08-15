#include "LightsOutCharacterHUD.h"
#include "Components/Image.h"
#include "Engine/Engine.h"


void ULightsOutCharacterHUD::SetItemSlot()
{
    UE_LOG(LogTemp, Warning, TEXT("Setting Item Slot"));
    Cast<UImage>(ItemSlot_1_Image)->SetOpacity(0.3f);
}

void ULightsOutCharacterHUD::RorderItems(const AItemBase& Item, int Index)
{

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
