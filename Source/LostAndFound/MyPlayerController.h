// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LOSTANDFOUND_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	ACharacter* PossessedChar;

	
	
private:
	//Setting up the Camera and Enabling the Character Movement
	virtual void OnPossess(APawn* InPawn) override;
	//Binding movement and action functions
	virtual void SetupInputComponent() override;

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
	void Interact();
	
};
