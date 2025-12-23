// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "HideAndSeekPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class LIGHTSOUT_API AHideAndSeekPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	virtual void PostInitializeComponents() override; 
};
