// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInventory.h"
#include "InventoryItem.h"

// Sets default values
APlayerInventory::APlayerInventory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	GiveItem("Conveyor", 50);
	GiveItem("Assembler", 50);
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
		if (GEngine)
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

bool APlayerInventory::GiveItem(SpawnableInfo * _Item, UINT32 amount)
{
	for (int i = 0; i < MaxInventorySlots; i++) {
		if (Contents.IsValidIndex(i)) {
			if (Contents[i]->ItemName == _Item->ItemName) {
				if (Contents[i]->Amount + amount <= _Item->MaxStackSize) {
					//SpawnableInfo* NewItem = _Item;
					Contents[i]->Amount += amount;
					//Contents[i]->Amount += amount;
					//delete Contents[i];
					//Contents.RemoveAt(i);
					//Contents.Insert(NewItem, i);
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (L"amount left: " + FString::FromInt(Contents[i]->Amount)));
					return true;
				}
			}
		} else {
			//broken slot?, unlikely, but fill with empty
		}
	}
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (L"No item found, Gave New."));
	//no slot was found
	for (int i = 0; i < MaxInventorySlots; i++) {
		if (GetItemInSlot(i)->ItemName == "Empty") {
			delete Contents[i];
			Contents.RemoveAt(i);
			Contents.Insert(_Item, i);
			Contents[i]->Amount = amount;
			return true;

		}
	}
	return false;
}

bool APlayerInventory::GiveItem(SpawnableInfo* _Item)
{
	return GiveItem(_Item, 1);
}

bool APlayerInventory::GiveItem(FString _Name, UINT32 amount)
{
	return GiveItem(GetSpawnableByName(_Name), amount);
}

bool APlayerInventory::GiveItem(FString _Name)
{
	return GiveItem(_Name, 1);
}

bool APlayerInventory::RemoveItem(int Index, UINT32 amount)
{
	delete Contents[Index];
	Contents.RemoveAt(Index);
	Contents.Insert(GetSpawnableByName("Empty"), Index);
	return true;
}

bool APlayerInventory::RemoveItem(int Index)
{
	return RemoveItem(Index, 1);
}

bool APlayerInventory::RemoveItem(FString _Name, UINT32 amount)
{
	for (int i = 0; i < MaxInventorySlots; i++) {
		if (Contents[i]->ItemName == _Name) {
			RemoveItem(i, amount);
			return true;
		}
	}
	return false;
}

bool APlayerInventory::RemoveItem(FString _Name)
{
	return RemoveItem(_Name, 1);
}

