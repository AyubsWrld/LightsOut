#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "ProceduralMeshComponent.h"
#include "Tile.generated.h"

UCLASS()
class LIGHTSOUT_API ATile : public AActor
{
	GENERATED_BODY()

public:
	ATile();


	UFUNCTION()
	void Invocable(AActor* Player);

	FProcMeshSection* Mesh;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};