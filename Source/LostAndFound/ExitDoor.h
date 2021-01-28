// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InteractInterface.h"
#include "MyGameStateBase.h"
#include "GameFramework/Actor.h"
#include "ExitDoor.generated.h"

UCLASS()
class LOSTANDFOUND_API AExitDoor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExitDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AMyGameStateBase* GameState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	UInteractInfo* InteractInfo;

	virtual void Interact_Implementation(AMyPlayerController* Controller) override;
	virtual UInteractInfo* GetInteractInfo_Implementation() override;
	

};
