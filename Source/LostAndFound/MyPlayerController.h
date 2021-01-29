// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InteractInterface.h"
#include "Item.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryChanged);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaunchWindupStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaunchStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLaunchEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewCommission);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCompletedCommission);

/**
 * 
 */
UCLASS(BlueprintType)
class LOSTANDFOUND_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	AMyPlayerController();

	ACharacter* PossessedChar;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UUserWidget> HUDWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, int32> ItemInventory;
	UFUNCTION(BlueprintCallable)
	void AddToInventory(UItem* Item);
	UPROPERTY(BlueprintAssignable)
	FInventoryChanged OnInventoryChange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* LastHoveredOverActor;
	UPROPERTY(BlueprintReadOnly)
	float LastHoverTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HoverCoyoteTime;
	UPROPERTY(BlueprintReadOnly)
	UInteractInfo* LastHoverData;

	UPROPERTY()
	FTransform HomePosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool LaunchMode;
	UPROPERTY(BlueprintReadOnly)
	ACameraActor* LaunchCamera;
	UFUNCTION(BlueprintCallable)
	bool EnterLaunchMode();
	UFUNCTION(BlueprintCallable)
    bool LeaveLaunchMode();
	UFUNCTION(BlueprintCallable)
    void ToggleLaunchMode();

	UPROPERTY(BlueprintReadOnly)
	bool DecidedToLaunch;
	UPROPERTY(BlueprintReadOnly)
	FVector LaunchStart;
	UPROPERTY(BlueprintReadOnly)
	FVector LaunchMidPoint;
	UPROPERTY(BlueprintReadOnly)
	FVector LaunchEnd;
	UPROPERTY(BlueprintReadOnly)
	float LaunchTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 LaunchState;
	UPROPERTY(BlueprintAssignable)
	FOnLaunchWindupStart OnLaunchWindup;
	UPROPERTY(BlueprintAssignable)
	FOnLaunchStart OnLaunchStart;
	UPROPERTY(BlueprintAssignable)
	FOnLaunchEnd OnLaunchEnd;

	UPROPERTY(BlueprintAssignable)
	FOnNewCommission OnNewCommission;
	UPROPERTY(BlueprintAssignable)
	FOnCompletedCommission OnCompletedCommission;
	
	UFUNCTION(BlueprintCallable)
	void GoHome();
	
private:
	//Setting up the Camera and Enabling the Character Movement
	virtual void OnPossess(APawn* InPawn) override;
	//Binding movement and action functions
	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	//Misc Movement Functions
	UFUNCTION()
	void MoveX(float mag);
	UFUNCTION()
	void MoveY(float mag);

	UFUNCTION()
	void MouseLookX(float mag);
	UFUNCTION()
	void MouseLookY(float mag);

	UFUNCTION()
	void JoyLookX(float mag);
	UFUNCTION()
	void JoyLookY(float mag);

	UFUNCTION()
    void Jump();

	UFUNCTION()
	void Interact();

	UFUNCTION()
    void Click();
};