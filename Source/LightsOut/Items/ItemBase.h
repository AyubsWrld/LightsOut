// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightsOut/Generics/Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "ItemBase.generated.h"

UCLASS()
class LIGHTSOUT_API AItemBase : public AActor, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY()
	FGuid IID;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated, Category = "Item")
	UStaticMesh* ItemMesh;

	AItemBase();
	virtual void Interact(FGuid Interactor) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	[[nodiscard]] FGuid GetID() const;
};
