// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"



#include "LostAndFoundGameModeBase.h"
#include "EngineUtils.h"
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

	for (TActorIterator<ASpawnLocation> It(GetWorld()); It; ++It)
	{
		SpawnLocations.Add(*It);
	}

	CommissionRows = CommissionsTable->GetRowNames();
	UItemLibrary::QueueRangeOfValues(CommissionRandomPool, 0, CommissionRows.Num() - 1);
	GenerateNewCommission();
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

void AMyGameStateBase::GenerateNewCommission()
{
	if (UItemLibrary::PoolLength(CommissionRandomPool) == 0)
	{
		UItemLibrary::QueueRangeOfValues(CommissionRandomPool, 0, CommissionRows.Num() - 1);
	}
	int32 ChosenRow;
	bool Picked = UItemLibrary::PopValueFromPool(CommissionRandomPool, ChosenRow);
	if (Picked)
	{
		auto FoundRow = CommissionsTable->FindRow<FCommissionData>(CommissionRows[ChosenRow], FString{TEXT("Generating new commission")});
		if (FoundRow)
		{
			UCommission* Commission = NewObject<UCommission>(this);
			Commission->RequiredItems = FoundRow->Requirements;
			Commission->SetBonusTimeReward(FoundRow->BonusTime);
			GetNewCommission(Commission);
			SpawnItemsFromCommission();
		}
	}
}

void AMyGameStateBase::SpawnItemsFromCommission()
{
	int32 AttemptsMax = 400.0f;
	int32 Attempts = 0;
	for (auto& Item : CurrentCommission->RequiredItems)
	{
		int32 SpawnAmount = Item.Value;
		for (int32 i = 0; i < SpawnAmount; ++i)
		{
			for (int32 j = 0; j < 10; ++j)
			{
				int32 Chosen = FMath::RandRange(0, SpawnLocations.Num() - 1);
				if (!SpawnLocations[Chosen]->OwnedItem.Get())
				{
					AWorldItem* WorldItem = GetWorld()->SpawnActor<AWorldItem>(SpawnLocations[Chosen]->GetActorLocation(), SpawnLocations[Chosen]->GetActorRotation());
					WorldItem->CreateInternalItemFromName(ItemTable, Item.Key);
					SpawnLocations[Chosen]->OwnedItem = WorldItem;
					break;
				}
				++Attempts;
			}
			if (Attempts >= AttemptsMax)
				break;
		}
		if (Attempts >= AttemptsMax)
			break;
	}
	check(Attempts < AttemptsMax);
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

