// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LightsOut/Items/ItemTypes.h"
#include "LightsOut/Items/ItemBase.h"
#include "Styling/SlateBrush.h"
#include "Components/Image.h"
#include "LightsOutCharacterHUD.generated.h"

#define MAX_INVENTORY_SIZE 4 

/**
 *
 */

UCLASS()

class LIGHTSOUT_API ULightsOutCharacterHUD : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FItemSlot> ItemSlots;

	const AItemBase* Items[4]{};

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

	UPROPERTY(EditAnywhere, Category=Iconography)
	UTexture2D* DefaultIcon; // Nullify for checking. 


	UWidget* CurrentActive{};

protected:

private:

public:

	void UpdateHUD(AItemBase* Item);

	/* Wrapper around SetContentForSlot */

	void SetItemSlot();

	void RorderItems(const AItemBase& Item, int Index);

	[[nodiscard]] bool IsWellFormed() const; // Invariance check; 
	
	[[nodiscard]] UImage* GetImageAtIndex(int32 Index) const;

	[[nodiscard]] UWidget* GetSlotAtIndex(int32 Index) const;

	[[nodiscard]] int32 GetEmptySlot() const;

	void RemoveAtIndex(int32 Index);

	void DebugItemSlot(int32 index);

	virtual void NativeConstruct() override; 

	ULightsOutCharacterHUD(const FObjectInitializer& ObjectInitializer);

};
