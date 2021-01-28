// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"


#include "LostAndFoundGameModeBase.h"
#include "MyPlayerController.h"

AMyGameStateBase::AMyGameStateBase()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMyGameStateBase::BeginPlay()
{
	
	GameState = GameState_MailRoom;
	const ALostAndFoundGameModeBase* GameModeBase = Cast<ALostAndFoundGameModeBase>(GetDefaultGameMode());
	if(GameModeBase)
	{
		TimeLeft = GameModeBase->BeginningTime;
	}

	PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	
	PlayerController->OnLaunchEnd.AddDynamic(this,&AMyGameStateBase::OnLanded);
	PlayerController->OnLaunchStart.AddDynamic(this,&AMyGameStateBase::OnStartLaunch);
	
}

void AMyGameStateBase::Tick(float DeltaSeconds)
{
	//GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Blue,UEnum::GetValueAsString(GameState));
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
	if(TimeLeft <= 0.0f)
	{
		GameState = GameState_PostGame;
	}
}

void AMyGameStateBase::OnStartLaunch()
{
	SetGameState(GameState_Launching);
}

void AMyGameStateBase::OnLanded()
{
	SetGameState(GameState_InGame);
}

void AMyGameStateBase::AddExtraTime(float ExtraTime)
{
	TimeLeft += ExtraTime;
}

void AMyGameStateBase::SetGameState(TEnumAsByte<EGameState> NewGameState)
{
	GameState = NewGameState;
}

bool AMyGameStateBase::GetNewCommission(UCommission* Commission)
{
	if (CurrentCommission == nullptr)
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

