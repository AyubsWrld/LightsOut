// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LightsOut/Items/ItemTypes.h"
#include "Inventory.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIGHTSOUT_API UInventory : public UActorComponent
{
	GENERATED_BODY()

	/* Easiest approach would just be to have the server validate the item later on*/
	TArray<AItemBase*> Items{}; 

	UPROPERTY(EditAnywhere)
	int32 ActiveItem{};

public:	
	// Sets default values for this component's properties
	UInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddItem(const AItemBase* Item); 

	void EquipItem(int32 Index) ;

	void DiscardItem(int32 Index);

	AItemBase* GetItemAtIndex(int32 Index) const;

	// void SwapItems(int32 Index);

};
