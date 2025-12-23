// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
/**
 * 
 */

/* What is the purpose of forwarding the reference? */
/* Would it make sense to just utilize header only functions? */ 

namespace LOUT
{
	enum class EWinCondition
	{
		WC_UNDEFINED,
		WC_TimeBased,
		WC_EliminationBased,
		WC_PuzzleBased,
		WC_MAX
	};

	enum class EMinigameDifficulty
	{
		MGD_UNDEFINED,
		MGD_Easy,
		MGD_Medium,
		MGD_Hard,
		MGD_Insane,
	};
	
	constexpr EWinCondition operator|(EWinCondition Lhs, EWinCondition Rhs);
	
	DECLARE_DELEGATE(FEndMingameDelegate);		
	DECLARE_DELEGATE(FStartMingiameDelegate);	
	
	using TEndMinigameDelegate		=	FEndMingameDelegate;
	using TStartMinigameDelegate	=	FEndMingameDelegate;
	
	class LIGHTSOUT_API TMinigame  : public AGameModeBase
	{ 
		struct IMinigameConcept 
		{
			virtual		~IMinigameConcept() = default; 
			virtual		TEndMinigameDelegate&	GetEndMinigameDelegate()	= 0;
			virtual		TStartMinigameDelegate& GetStartMinigameDelegate()	= 0;
		};
		
		template<typename T>
		struct IMinigameErasedContainer : public IMinigameConcept
		{
			IMinigameErasedContainer(const T& object)
				: IMinigameConcept(object)
			{}
			
			virtual		TEndMinigameDelegate&		GetEndMinigameDelegate()	override; 
			virtual		TStartMinigameDelegate&		GetStartMinigameDelegate()	override; 
			
			T type_erased_object_;
		};

	public:
		
		template<typename T>
		TMinigame(T&& Minigame)
			: MinigamePointer(&Minigame)
		{}
		
		TEndMinigameDelegate&		GetEndMinigameDelegate();
		TStartMinigameDelegate&		GetStartMinigameDelegate();
		
	private:
		IMinigameConcept* MinigamePointer;
	};
	
}
