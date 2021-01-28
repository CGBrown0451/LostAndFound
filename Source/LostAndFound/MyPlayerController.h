// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InteractInterface.h"
#include "Item.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryChanged);

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FName, int32> ItemInventory;
	UFUNCTION(BlueprintCallable)
	void AddToInventory(UItem* Item);
	FInventoryChanged OnInventoryChange;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* LastHoveredOverActor;
	UPROPERTY(BlueprintReadOnly)
	float LastHoverTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HoverCoyoteTime;
	UPROPERTY(BlueprintReadOnly)
	UInteractInfo* LastHoverData;
	
private:
	//Setting up the Camera and Enabling the Character Movement
	virtual void OnPossess(APawn* InPawn) override;
	//Binding movement and action functions
	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;

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
	
};