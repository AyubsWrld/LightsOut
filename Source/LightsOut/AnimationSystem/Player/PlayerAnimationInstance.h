// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimationInstance.generated.h"

class ALightsOutCharacter;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class LIGHTSOUT_API UPlayerAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	virtual void NativeInitializeAnimation() override;
	
	UFUNCTION(BlueprintCallable)
	void UpdateAnimProoperties(float DeltaTime);

	/* Pointer to player character */ 
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ALightsOutCharacter> LightsOutCharacter;

	/* "Don't save on disk?", references player movement character*/ 
	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	bool	bIsInAir			{false};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	bool	bIsAccelerating		{false};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	bool	bIsMoving			{false};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player, Meta = (AllowPrivateAccess = "true"))
	float	Speed				{0.0f}; 
};
