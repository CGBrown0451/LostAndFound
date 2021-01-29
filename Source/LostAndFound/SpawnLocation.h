// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "WorldItem.h"
#include "GameFramework/Actor.h"
#include "SpawnLocation.generated.h"

UCLASS()
class LOSTANDFOUND_API ASpawnLocation : public AActor
{
	GENERATED_BODY()
	public:
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AWorldItem> OwnedItem;
	
public:	
	// Sets default values for this actor's properties
	ASpawnLocation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
