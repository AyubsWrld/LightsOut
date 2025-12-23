// Fill out your copyright notice in the Description page of Project Settings.

#include "MinigameManager.h"

bool UMinigameManager::ShouldCreateSubsystem(UObject* Outer) const 
{
	const UWorld* World = Cast<UWorld>(Outer);
	if (!World || !World->IsGameWorld()) return false;

	const ENetMode  NetMode = World->GetNetMode();
	return (NetMode == NM_DedicatedServer || NetMode == NM_ListenServer);
}

void UMinigameManager::OnWorldBeginPlay(UWorld& InWorld)
{ 
	/* Do we want to set up something for the client or not? */ 
	if (InWorld.GetNetMode() == ENetMode::NM_Client)
		return;
	Super::OnWorldBeginPlay(InWorld);
	BindGameModeDelegates();
}

void UMinigameManager::ChangeMinigame()
{
	UE_LOG(LogTemp, Warning, TEXT("[UMinigameManager]: ChangeMinigame"));
	GetWorld()->ServerTravel(
		"HideAndSeek",
		false,
		false
		);
	return;
}

void UMinigameManager::BeginMinigame(LOUT::FMinigame& InMinigame)
{
	bInMinigame = true;
	return;
}

void UMinigameManager::OnMinigameEnd(LOUT::FMinigame& InMinigame)
{
	return;
}

void UMinigameManager::BindGameModeDelegates()
{
	if (InClientContext())
		return;
	if (AHideAndSeekGameMode* GameModeReference{ Cast<AHideAndSeekGameMode>(GetWorld()->GetAuthGameMode())} ; GameModeReference )
	{
		UE_LOG(LogTemp, Warning, TEXT("Bound Delegates"))
		auto& MinigameEndDelegate{ GameModeReference->GetEndMinigameDelegate() } ;
		MinigameEndDelegate.BindLambda([this](){ChangeMinigame();});
		return; 
	}
	UE_LOG(LogTemp, Error, TEXT("Failed to Bind Delegates"))
}
