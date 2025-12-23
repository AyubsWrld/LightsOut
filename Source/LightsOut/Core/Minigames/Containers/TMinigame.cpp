// Fill out your copyright notice in the Description page of Project Settings.


#include "LightsOut/Core/Minigames/Containers/TMinigame.h"

namespace LOUT
{
	
	constexpr EWinCondition operator|(EWinCondition Lhs, EWinCondition Rhs)
	{
		using	TUnderlying		=	std::underlying_type_t<EWinCondition>;
		return static_cast<EWinCondition>(static_cast<TUnderlying>(Lhs) | static_cast<TUnderlying>(Rhs));
	}
	
	template <typename T>
	TEndMinigameDelegate& TMinigame::IMinigameErasedContainer<T>::GetEndMinigameDelegate()
	{
		return type_erased_object_.GetEndMinigameDelegate();
	}
	
	template <typename T>
	TStartMinigameDelegate& TMinigame::IMinigameErasedContainer<T>::GetStartMinigameDelegate()
	{
		return type_erased_object_.GetStartMinigameDelegate();
	}
	
	TEndMinigameDelegate& TMinigame::GetEndMinigameDelegate()
	{
		UE_LOG(LogTemp, Warning, TEXT("%p"), MinigamePointer);
		check(MinigamePointer && "Could not retrieve pointer to minigame");
		return MinigamePointer->GetEndMinigameDelegate();
	}
	
	TStartMinigameDelegate&	TMinigame::GetStartMinigameDelegate()
	{
		UE_LOG(LogTemp, Warning, TEXT("%p"), MinigamePointer);
		check(!MinigamePointer && "Could not retrieve pointer to minigame");
		return MinigamePointer->GetStartMinigameDelegate();
	}
	
}
