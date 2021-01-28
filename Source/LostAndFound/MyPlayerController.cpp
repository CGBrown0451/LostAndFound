// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"


#include "InteractInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyPlayerController::AMyPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	HoverCoyoteTime = 0.3f;
}

void AMyPlayerController::AddToInventory(UItem* Item)
{
	auto Found = ItemInventory.Find(Item->ItemType);
	if (Found)
		ItemInventory.Add(Item->ItemType, *Found + 1);
	else
		ItemInventory.Add(Item->ItemType, 1);
	OnInventoryChange.Broadcast();
	UE_LOG(LogTemp, Display, TEXT("%s added to inventory"), *Item->ItemType.ToString());
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AMyPlayerController::Interact);
	
	InputComponent->BindAxis(TEXT("MoveX"),this,&AMyPlayerController::MoveX);
	InputComponent->BindAxis(TEXT("MoveY"),this,&AMyPlayerController::MoveY);

	InputComponent->BindAxis(TEXT("MouseLookX"),this,&AMyPlayerController::MouseLookX);
	InputComponent->BindAxis(TEXT("MouseLookY"),this,&AMyPlayerController::MouseLookY);

	InputComponent->BindAxis(TEXT("JoyLookX"),this,&AMyPlayerController::JoyLookX);
	InputComponent->BindAxis(TEXT("JoyLookY"),this,&AMyPlayerController::JoyLookY);
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector{10, 10, 10});
	FHitResult SweepResult;
	FVector Start;
	FRotator Rotation;
	PossessedChar->GetActorEyesViewPoint(Start, Rotation);
	FVector End = Start + PlayerCameraManager->GetActorForwardVector() * 300.0f;

	// Check any sign of interact actors from eye of player to some distance away
	bool HasHit = GetWorld()->SweepSingleByChannel(SweepResult, Start, End, Rotation.Quaternion(), ECollisionChannel::ECC_GameTraceChannel1, Shape);
	if (HasHit)
	{
		// Actor must implement the interact interface
		if (SweepResult.GetActor() && SweepResult.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			// Retrieve interact info (e.g. tooltip)
			UInteractInfo* Info = IInteractInterface::Execute_GetInteractInfo(SweepResult.GetActor());
			if (Info)
			{
				LastHoverTime = GetWorld()->GetTimeSeconds() + HoverCoyoteTime;
				LastHoveredOverActor = SweepResult.GetActor();
				LastHoverData = Info;
			}
		}
	}
	// Make hovered over actors expire after some time of not being hovered over
	if (LastHoveredOverActor && LastHoverTime < GetWorld()->GetTimeSeconds())
	{
		LastHoveredOverActor = nullptr;
	}
}

void AMyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PossessedChar = Cast<ACharacter>(InPawn);
	if(PossessedChar)
	{
		PossessedChar->GetCharacterMovement()->SetActive(true,true);
		SetViewTargetWithBlend(PossessedChar,0.0f,VTBlend_Linear);
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1,100.0f,FColor::Red,"FU");
	}
}

void AMyPlayerController::MoveX(float mag)
{
	PossessedChar->AddMovementInput(PossessedChar->GetActorRightVector() * mag, 1.0f, false);
}

void AMyPlayerController::MoveY(float mag)
{
	PossessedChar->AddMovementInput(PossessedChar->GetActorForwardVector() * mag, 1.0f, false);
}

void AMyPlayerController::MouseLookX(float mag)
{
	AddYawInput(mag);
}

void AMyPlayerController::MouseLookY(float mag)
{
	AddPitchInput(mag);
}

void AMyPlayerController::JoyLookX(float mag)
{
	AddYawInput(mag);
}

void AMyPlayerController::JoyLookY(float mag)
{
	AddPitchInput(mag);
}

/**
 * @brief Called when the player presses the interact button
 */
void AMyPlayerController::Interact()
{
	// Check if an interact actor has been hovered over recently
	if (LastHoveredOverActor && LastHoverData && LastHoverData->CanInteract)
	{
		// Execute its interact
		IInteractInterface::Execute_Interact(LastHoveredOverActor, this);
		// Optionally destroy the actor if it signals it (e.g. picking up items)
		if (LastHoverData->DestroyAfterInteract)
		{
			LastHoveredOverActor->Destroy();
			LastHoveredOverActor = nullptr;
		}
	}
}