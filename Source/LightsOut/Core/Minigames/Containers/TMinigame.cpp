// Fill out your copyright notice in the Description page of Project Settings.


#include "LightsOut/Core/Minigames/Containers/TMinigame.h"

namespace LO
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
		return minigame_pointer_->GetEndMinigameDelegate();
	}
	
	TStartMinigameDelegate&	TMinigame::GetStartMinigameDelegate()
	{
		return minigame_pointer_->GetStartMinigameDelegate();
	}
	
}
