// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "InteractInterface.h"
#include "Item.h"
#include "ItemLibrary.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "WorldItem.generated.h"

UCLASS()
class LOSTANDFOUND_API AWorldItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldItem();

	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UItem* InternalItem;
	FItemData* LastDataFound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool CreateInternalItemFromName(UDataTable* FromTable, FName InName);

	UPROPERTY()
	UInteractInfo* InteractInfo;
	virtual void Interact_Implementation(AMyPlayerController* Controller) override;
    virtual UInteractInfo* GetInteractInfo_Implementation() override;
};
