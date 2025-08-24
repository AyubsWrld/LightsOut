#include "ItemBase.h"
#include "LightsOut/Core/IBSingleton.h"
#include "LightsOut/Items/ItemTypes.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AItemBase::AItemBase()
{
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


void AItemBase::Interact(FGuid Interactor)
{
	/* Should be executing within the context of the server */
	if (GetNetMode() == ENetMode::NM_Client)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s]: Executing within client context"), ANSI_TO_TCHAR(__FUNCTION__));
		return;
	}

	UIBSingleton* singleton = GetWorld()->GetSubsystem<UIBSingleton>();
	if (singleton->TryAddToPlayerInventory(*this, Interactor))
		return; 
}


[[nodiscard]] FGuid AItemBase::GetID() const
{
	return FGuid::NewGuid();
}

[[nodiscard]] FString AItemBase::GetThumbnailURI() const
{
	return TEXT("STUB");
}

void AItemBase::Use()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: STUB CALLED"), ANSI_TO_TCHAR(__FUNCTION__));
}

void AItemBase::Equip()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: STUB CALLED"), ANSI_TO_TCHAR(__FUNCTION__));
}

void AItemBase::Drop(const FVector&& Location)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s]: STUB CALLED"), ANSI_TO_TCHAR(__FUNCTION__));
}


EItemCategory AItemBase::GetType() const
{
	return EItemCategory::IC_Undefined; 
}

[[nodiscard]] UTexture2D* AItemBase::GetIcon() const
{
	return this->Icon; 
}
