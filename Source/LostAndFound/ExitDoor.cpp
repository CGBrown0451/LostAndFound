// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitDoor.h"


#include "MyGameStateBase.h"
#include "MyPlayerController.h"

// Sets default values
AExitDoor::AExitDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->ResetRelativeTransform();
	MeshComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

}

// Called when the game starts or when spawned
void AExitDoor::BeginPlay()
{
	Super::BeginPlay();

	GameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
	InteractInfo = NewObject<UInteractInfo>(this);
	
}

// Called every frame
void AExitDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExitDoor::Interact_Implementation(AMyPlayerController* Controller)
{
	GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Black,"E");
	Controller->GoHome();
	if(GameState)
	{
		GameState->SetGameState(GameState_MailRoom);
	}
}

UInteractInfo* AExitDoor::GetInteractInfo_Implementation()
{
	return InteractInfo;
}

