// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimationInstance.h"

#include "Engine/Light.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LightsOut/LightsOutCharacter.h"


void UPlayerAnimationInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	/* Shooter character might be null here */ 
	
	if (APawn* PlayerPawn  = TryGetPawnOwner() ; PlayerPawn )
	{
		LightsOutCharacter	= Cast<ALightsOutCharacter>(PlayerPawn);
		MovementComponent	= LightsOutCharacter ?  LightsOutCharacter->GetCharacterMovement() : nullptr;  
	}
}
	
void UPlayerAnimationInstance::UpdateAnimProoperties(float DeltaTime)
{
	// If it is invalid recatch? I guess if we change characters. 
	APawn* OwnerNow = TryGetPawnOwner();
	if ( OwnerNow  != LightsOutCharacter || !IsValid(MovementComponent) )
	{
		LightsOutCharacter	= Cast<ALightsOutCharacter>(OwnerNow);
		MovementComponent	= LightsOutCharacter ?  LightsOutCharacter->GetCharacterMovement() : nullptr;  
	}
	if ( !LightsOutCharacter || !MovementComponent ) return ;
	
	const FVector Velocity = LightsOutCharacter->GetVelocity();
	Speed = Velocity.Size2D(); 
	
	// is in air? 
	
	bIsInAir		=		MovementComponent->IsFalling();

	// Velocity tells us how fast the character is going, the acceleration is whether the player is actively pressing something. 
	// is player actively providing movement input? acceleration is whether the character is actively providing input 
	bIsAccelerating =	MovementComponent->GetCurrentAcceleration().SizeSquared() > KINDA_SMALL_NUMBER;
}
