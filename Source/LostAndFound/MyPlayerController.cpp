// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"


#include "InteractInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
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
	FVector Start = PossessedChar->GetActorLocation();
	FVector End = Start + PlayerCameraManager->GetActorForwardVector() * 300.0f;
	// TODO: Doesn't work, look into it
	bool HasHit = GetWorld()->SweepSingleByChannel(SweepResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_EngineTraceChannel1, Shape);
	if (HasHit)
	{
		if (SweepResult.GetActor() && SweepResult.GetActor()->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		{
			UInteractInfo* Info = IInteractInterface::Execute_GetInteractInfo(SweepResult.GetActor());
			if (Info)
				UE_LOG(LogTemp, Display, TEXT("Hovering: %s"), *Info->Tooltip)
		}
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
	
}

void AMyPlayerController::JoyLookY(float mag)
{
	
}

void AMyPlayerController::Interact()
{
}