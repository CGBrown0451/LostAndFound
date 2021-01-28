// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include <future>



#include "InteractInterface.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
	InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyPlayerController::Jump);
	InputComponent->BindAction(TEXT("Debug_LaunchMode"), EInputEvent::IE_Pressed, this, &AMyPlayerController::ToggleLaunchMode);
	InputComponent->BindAction(TEXT("Click"), EInputEvent::IE_Pressed, this, &AMyPlayerController::Click);
	
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
	// Launch states
	switch(LaunchState)
	{
		case 1:
			if (LaunchTime + 1.4f <= GetWorld()->GetTimeSeconds())
			{
				LaunchTime = GetWorld()->GetTimeSeconds();
				LaunchState = 2;
			}
			break;
		case 2:
			if (LaunchTime + 1.8f > GetWorld()->GetTimeSeconds())
			{
				GetCharacter()->GetCharacterMovement()->GravityScale = 0.0f;
			}
			else
			{
				GetCharacter()->GetCharacterMovement()->GravityScale = 1.0f;
				LaunchState = 0;
			}
			float Alpha = FMath::Clamp((GetWorld()->GetTimeSeconds() - LaunchTime) / 1.8f, 0.0f, 1.0f);
			PossessedChar->SetActorLocation(
        	    FMath::Lerp(FMath::Lerp(LaunchStart, LaunchMidPoint, Alpha),
        	    FMath::Lerp(LaunchMidPoint, LaunchEnd, Alpha), Alpha)
        	);
			break;
	}
	
}

void AMyPlayerController::BeginPlay()
{
	LaunchCamera = GetWorld()->SpawnActor<ACameraActor>();
	LaunchCamera->GetCameraComponent()->bConstrainAspectRatio = false;
}

bool AMyPlayerController::EnterLaunchMode()
{
	if (!LaunchMode)
	{
		LaunchCamera->SetActorTransform(PossessedChar->GetTransform());
		LaunchCamera->AddActorLocalRotation(FRotator{-89.9f, 0, 0});
		LaunchCamera->AddActorWorldOffset(FVector{0, 0, 10000.0f});
		SetViewTargetWithBlend(LaunchCamera,0.5f,VTBlend_EaseInOut, 2.0f, true);
		PossessedChar->DisableInput(this);
		SetShowMouseCursor(true);
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
		SetInputMode(FInputModeGameAndUI{});
		LaunchMode = true;
		return true;
	}
	return false;
}

bool AMyPlayerController::LeaveLaunchMode()
{
	if (LaunchMode)
	{
		LaunchMode = false;
		PossessedChar->EnableInput(this);
		SetViewTargetWithBlend(PossessedChar,0.5f,VTBlend_EaseInOut, 2.0f, true);
		SetShowMouseCursor(false);
		bShowMouseCursor = false;
		bEnableClickEvents = false;
		bEnableMouseOverEvents = false;
		SetInputMode(FInputModeGameOnly{});
		return true;
	}
	return false;
}

void AMyPlayerController::ToggleLaunchMode()
{
	if (LaunchState == 0)
	{
		if (LaunchMode)
			LeaveLaunchMode();
		else
			EnterLaunchMode();
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
		GetCharacter()->GetCharacterMovement()->AirControl = 0.70f;
		GetCharacter()->GetCharacterMovement()->BrakingDecelerationFalling = 1000.0f;
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1,100.0f,FColor::Red,"FU");
	}
}

void AMyPlayerController::MoveX(float mag)
{
	if (!LaunchMode && LaunchState == 0)
		PossessedChar->AddMovementInput(PossessedChar->GetActorRightVector() * mag, 1.0f, false);
}

void AMyPlayerController::MoveY(float mag)
{
	if (!LaunchMode && LaunchState == 0)
		PossessedChar->AddMovementInput(PossessedChar->GetActorForwardVector() * mag, 1.0f, false);
}

void AMyPlayerController::MouseLookX(float mag)
{
	if (!LaunchMode)
		AddYawInput(mag);
}

void AMyPlayerController::MouseLookY(float mag)
{
	if (!LaunchMode)
		AddPitchInput(mag);
}

void AMyPlayerController::JoyLookX(float mag)
{
	if (!LaunchMode && LaunchState == 0)
		AddYawInput(mag);
}

void AMyPlayerController::JoyLookY(float mag)
{
	if (!LaunchMode && LaunchState == 0)
		AddPitchInput(mag);
}

void AMyPlayerController::Jump()
{
	if (!LaunchMode && LaunchState == 0)
		PossessedChar->Jump();
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

void AMyPlayerController::Click()
{
	if (LaunchMode)
	{
		FHitResult Result;
		bool HasHit = GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery3, true, Result);
		if (HasHit)
		{
			LaunchStart = PossessedChar->GetActorLocation();
			FVector EyeLocation;
			FRotator EyeRotation;
			PossessedChar->GetActorEyesViewPoint(EyeLocation, EyeRotation);
			LaunchEnd = Result.Location + EyeLocation - PossessedChar->GetActorLocation();
			LaunchMidPoint = LaunchStart / 2 + LaunchEnd / 2 + FVector::UpVector * 3000.0f;
			DecidedToLaunch = true;
			LaunchTime = GetWorld()->GetTimeSeconds();
			LaunchState = 1;
			FRotator PlayerRot = UKismetMathLibrary::FindLookAtRotation(PossessedChar->GetActorLocation(), Result.Location);
			SetControlRotation(PlayerRot);
			LeaveLaunchMode();
		}
	}
}
