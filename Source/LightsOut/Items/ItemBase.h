// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightsOut/Generics/Interactable.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "ItemBase.generated.h"

enum class EItemCategory : uint8;

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

	UPROPERTY(EditAnywhere, Category=Icon)
	UTexture2D* Icon;

	AItemBase();
	virtual ~AItemBase() = default;

	virtual void Interact(APlayerState* Player) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override; // Needed? 

 	[[nodiscard]] virtual FGuid GetID() const;

	[[nodiscard]] virtual UTexture2D* GetIcon() const ; 
	[[nodiscard]] virtual FString GetThumbnailURI() const;

	virtual void Use();

	virtual void Equip();

	virtual void Drop(const FVector&& Location);

	virtual EItemCategory GetType() const;

};
