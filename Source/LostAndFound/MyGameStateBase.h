// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Commission.h"
#include "MyPlayerController.h"
#include "ItemLibrary.h"
#include "SpawnLocation.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

UENUM(BlueprintType)
enum EGameState
{
	GameState_MailRoom,
	GameState_Launching,
	GameState_InGame,
	GameState_PostGame
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChanged, TEnumAsByte<EGameState>, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameTimeTick, int32, Time);

/**
 * 
 */
UCLASS()
class LOSTANDFOUND_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMyGameStateBase();
	
private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void InGameTick(float DeltaSeconds);
	
	UFUNCTION()
	void OnStartLaunch();
	UFUNCTION()
	void OnLanded();

	AMyPlayerController* PlayerController;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCommission* CurrentCommission;
	UPROPERTY(BlueprintReadOnly)
	float TimeLeft;
	UPROPERTY(BlueprintReadOnly)
	float Score;
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EGameState> GameState;
	
	UPROPERTY(BlueprintAssignable)
	FOnGameStateChanged GameStateChanged;
	UPROPERTY(BlueprintAssignable)
	FOnGameTimeTick GameTimeTick;

	UPROPERTY(BlueprintReadOnly)
	FCommissionData LastCommissionData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* CommissionsTable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FName> CommissionRows;
	UPROPERTY()
	FRandomPool CommissionRandomPool;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* ItemTable;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<ASpawnLocation*> SpawnLocations;

	//Adds Extra Time
	void AddExtraTime(float ExtraTime);

	//Sets Game State
	UFUNCTION(BlueprintCallable)
	void SetGameState(TEnumAsByte<EGameState> NewGameState);

	//Gets a New Commission, returns whether the commission was added or not
	UFUNCTION(BlueprintCallable)
	bool GetNewCommission(UCommission* Commission);
	UFUNCTION(BlueprintCallable)
    void GenerateNewCommission();
	UFUNCTION(BlueprintCallable)
    void SpawnItemsFromCommission();

	//Turns in the current commission, returns whether it can be completed or not
	bool TurnInCommission();
	
};
