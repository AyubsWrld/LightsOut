// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractorComponent.h"

#include "ActorReferencesUtils.h"
#include "Camera/CameraComponent.h"

UInteractorComponent::UInteractorComponent()
{
}


void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	/*
		 TODO: Potential bug because there is an owner chain. We have to cap at top level
		 AActor Parent or assert somehow std::is_convertible_to ALightsOutCharacter (T) make it genenric.
		 GetOwner()->GetOwner() is a valid call.
	 */
	
	Owner			=			GetOwner(); 
	
	BindInteractionManager();
	
}

void UInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInteractorComponent::OnInteract()
{
	/* At this point owner should always be valid and we should prefer crashing as opposed to handling erronous states */ 
	UE_LOG(LogTemp, Warning, TEXT("Player(%d) tried Interacting"), Cast<AActor>(Owner)->GetActorGuid().A);
	GetObjectOfInterest(); 
}

void UInteractorComponent::BindInteractionManager()
{
	InteractionManager = GetWorld()->GetSubsystem<UInteractionManager>();
	
	if ( !InteractionManager )
	{
		UE_LOG(LogTemp, Error, TEXT("[%s]: Could not bind InteractionManager"), ANSI_TO_TCHAR(__FUNCTION__));
	}

	InteractionDelegate.AddWeakLambda(
		InteractionManager,
		[&](FInteractionEvent Event){ InteractionManager->OnInteractionEvent(Event); }
	);
}


[[nodiscard]] UObject* UInteractorComponent::GetObjectOfInterest() const
{
	FHitResult				HitResult;
	AActor*					HitActor			{ nullptr };
	const UWorld*			World				{ GetWorld() };
	const FVector&			StartLocation		{ OwnerCamera->GetComponentLocation() };
	const FVector&			EndLocation			{ StartLocation + ( OwnerCamera->GetForwardVector() * InteractionRange )};
	FCollisionQueryParams   CollisionParameters {};

	CollisionParameters.AddIgnoredActor(Owner);
	
	DrawDebugLine(
		World,
		StartLocation,
		EndLocation,
		FColor::Red,
		true,
		4.0f,
		0,
		3
	);

	if ( World->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_WorldStatic,
		CollisionParameters
		) )
	{
		if ( HitActor = HitResult.GetActor() ; HitActor)
		{
		}
	}
	
	return  nullptr; 
	
}

[[nodiscard]] FInteractionEvent UInteractorComponent::GetStubEvent()  const
{
	return FInteractionEvent{nullptr, nullptr};
}


