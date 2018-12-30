// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInventory.h"
#include "InventoryItem.h"

// Sets default values
APlayerInventory::APlayerInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ActiveInventorySlot = 0; //0 to 9
	Contents.SetNum(10, false);
	UsingItem = false;
}

// Called when the game starts or when spawned
void APlayerInventory::BeginPlay()
{
	Super::BeginPlay();
	GiveItem(Spawnables::GetSpawnable(0));
}

// Called every frame
void APlayerInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APlayerInventory::IsItemPlaceable()
{
	return false;
}

uint32 APlayerInventory::GetActiveSlot()
{
	return ActiveInventorySlot;
}

bool APlayerInventory::IsUsingItem()
{
	return UsingItem;
}

bool APlayerInventory::Scroll(int Amount) {
	if (UsingItem == false) {
		int currentSlot = ActiveInventorySlot;
		int newvalue = ActiveInventorySlot + Amount;
		ActiveInventorySlot = FMath::Clamp(newvalue, 0, 9);
		if (currentSlot != ActiveInventorySlot) {
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void APlayerInventory::GiveItem(SpawnableInfo* _Item)
{
	for (int i = 0; i < 9; i++) {
		if (!GetItemInSlot(i)) {
			Contents[i] = _Item;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, Contents[i]->ItemName);
			return;
		}
	}
}

