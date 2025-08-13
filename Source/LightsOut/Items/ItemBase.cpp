#include "ItemBase.h"
#include "LightsOut/Core/IBSingleton.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AItemBase::AItemBase()
{
	// Assign Mesh
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	Mesh->SetupAttachment(Root);

	ItemMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicateMovement(true);

	// Assign IID
	IID = FGuid::NewGuid();
}

void AItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	/* Only replicate the mesh asset, not the components */
	DOREPLIFETIME(AItemBase, ItemMesh);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (ItemMesh && Mesh)
	{
		Mesh->SetStaticMesh(ItemMesh);
	}
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

[[nodiscard]] FGuid AItemBase::GetID() const
{
	return this->IID;
}

void AItemBase::Interact(FGuid Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("[Interaction] %s Interacted with Item"), *Interactor.ToString());
	UIBSingleton* singleton = GetWorld()->GetSubsystem<UIBSingleton>();
	singleton->AddToPlayerInventory(*this, Interactor);
}
