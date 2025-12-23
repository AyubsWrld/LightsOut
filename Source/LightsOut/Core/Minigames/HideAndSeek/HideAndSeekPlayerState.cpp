// Fill out your copyright notice in the Description page of Project Settings.


#include "HideAndSeekPlayerState.h"

void AHideAndSeekPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Warning, TEXT("Initialized Player"));
}

