// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"

#include "MyPlayerController.h"

void AMyGameStateBase::BeginPlay()
{
	GameState = GameState_MailRoom;
}

void AMyGameStateBase::Tick(float DeltaSeconds)
{
	switch (GameState)
	{
		case GameState_MailRoom:
		break;
		case GameState_Launching:
		break;
		case GameState_InGame:
			InGameTick(DeltaSeconds);
		break;
		case GameState_PostGame:
		break;
	}
}

void AMyGameStateBase::InGameTick(float DeltaSeconds)
{
	TimeLeft -= DeltaSeconds;
	Score += DeltaSeconds * 10;
	if(TimeLeft >= 0.0f)
	{
		GameState = GameState_PostGame;
	}
}

void AMyGameStateBase::AddExtraTime(float ExtraTime)
{
	TimeLeft += ExtraTime;
}

bool AMyGameStateBase::GetNewCommission(UCommission* Commission)
{
	if (Commission == nullptr)
	{
		CurrentCommission = Commission;
		return true;
	}
	return false;
}

bool AMyGameStateBase::TurnInCommission()
{
	AMyPlayerController* Player = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if(!Player)
	{
		return false;
	}
	
	if(CurrentCommission->InventoryHasAllItems(Player->ItemInventory))
	{
		CurrentCommission->RemoveRequiredItemsFromInventory(Player->ItemInventory);
		AddExtraTime(CurrentCommission->GetBonusTimeReward());
		//TODO: Score???
		CurrentCommission = nullptr;
		return true;
	}
	return false;
	
}

