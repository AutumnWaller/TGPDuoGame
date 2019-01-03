// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInventory.h"
#include "InventoryItem.h"

// Sets default values
APlayerInventory::APlayerInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ActiveInventorySlot = 0; //0 to MaxInventorySlots
	UsingItem = false;

	for (int i = 0; i < MaxInventorySlots; i++) {
		Contents.Add(GetSpawnableByName("Empty"));
	}
}

// Called when the game starts or when spawned
void APlayerInventory::BeginPlay()
{
	Super::BeginPlay();
	//GiveItem(Spawnables::GetSpawnable(0));
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

bool APlayerInventory::IsUsingItem()
{
	return UsingItem;
}

bool APlayerInventory::Scroll(int Amount) {
	if (UsingItem == false) {
		int currentSlot = ActiveInventorySlot;
		int newvalue = ActiveInventorySlot + Amount;
		ActiveInventorySlot = FMath::Clamp(newvalue, 0, MaxInventorySlots);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(currentSlot));

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

SpawnableInfo * APlayerInventory::GetSpawnableByName(FString _Name)
{
	for (int i = 0; i < Spawnables::AvailableSpawnables.Num(); i++) {
		if (Spawnables::AvailableSpawnables[i].ItemName == _Name)
			return new SpawnableInfo(Spawnables::AvailableSpawnables[i]);
	}
	return nullptr;
}

bool APlayerInventory::GiveItem(SpawnableInfo* _Item)
{
	for (int i = 0; i < MaxInventorySlots; i++) {
		if (GetItemInSlot(i)->ItemName == "Empty") {
			delete Contents[i];
			Contents.RemoveAt(i);
			Contents.Insert(_Item, i);
			return true;
		}else if (GetItemInSlot(i)->ItemName == _Item->ItemName) {
			if (_Item->Amount + 1 < _Item->MaxStackSize) {
				delete Contents[i];
				SpawnableInfo* NewItem = _Item;
				NewItem->Amount++;
				Contents.RemoveAt(i);
				Contents.Insert(_Item, i);
				return true;
			}
		}
			
		
	}
	return false;
}

bool APlayerInventory::GiveItem(FString _Name)
{
	return GiveItem(GetSpawnableByName(_Name));
}

bool APlayerInventory::RemoveItem(int Index)
{
	delete Contents[Index];
	Contents.RemoveAt(Index);
	Contents.Insert(GetSpawnableByName("Empty"), Index);
	return true;
}

bool APlayerInventory::RemoveItem(FString _Name)
{
	for (int i = 0; i < MaxInventorySlots; i++) {
		if (Contents[i]->ItemName == _Name) {
			RemoveItem(i);
			return true;
		}
	}
	return false;
}

