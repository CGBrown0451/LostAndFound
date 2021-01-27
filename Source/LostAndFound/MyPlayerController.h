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
	void MoveX(float mag);
	void MoveY(float mag);

	void MouseLookX(float mag);
	void MouseLookY(float mag);

	void JoyLookX(float mag);
	void JoyLookY(float mag);

	void Interact();
	
};
