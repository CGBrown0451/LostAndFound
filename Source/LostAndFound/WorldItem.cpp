// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldItem.h"

// Sets default values
AWorldItem::AWorldItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->ResetRelativeTransform();
}

// Called when the game starts or when spawned
void AWorldItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AWorldItem::CreateInternalItemFromName(UDataTable* FromTable, FName InName)
{
	FItemData* ItemFound = FromTable->FindRow<FItemData>(InName, *FString{TEXT("Creating Internal Item From Name")});
	if (ItemFound)
	{
		InternalItem = NewObject<UItem>(this, ItemFound->Class);
		InternalItem->ItemType = InName;
		LastDataFound = ItemFound;
		MeshComponent->SetStaticMesh(ItemFound->Mesh);
		return true;
	}
	return false;
}

