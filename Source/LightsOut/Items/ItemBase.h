// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightsOut/Generics/Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "ItemBase.generated.h"

UCLASS()
class LIGHTSOUT_API AItemBase : public AActor, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY() 
	FGuid IID; 

public:	
	// Sets default values for this actor's properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh; 
	AItemBase();

	virtual void Interact(FGuid Interactor) override; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	[[nodiscard]] FGuid GetID() const; 

};
