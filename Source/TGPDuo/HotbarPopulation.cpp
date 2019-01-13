// Fill out your copyright notice in the Description page of Project Settings.

#include "HotbarPopulation.h"
#include "TGPDuoCharacter.h"

void UHotbarPopulation::Initialise()
{ 
	//TODO: REMOVE NEED FOR PLACEHOLDER
	PlaceholderInventory = GetWorld()->SpawnActor<APlayerInventory>();
	GetInventory();
}

void UHotbarPopulation::GetInventory() {

	//TODO: FIND A WAY TO NOT HAVE TO DO THIS OK THANKS
	APlayerController * PlayerController = GetOwningPlayer();
	ATGPDuoCharacter * Char = Cast<ATGPDuoCharacter>(PlayerController->GetPawn());
	if (Char->Inventory) {
		Inventory = Char->Inventory;
	}
	else {
		Inventory = PlaceholderInventory;
	}
}
/*
	Inventory = nullptr;
	//
}

*/

bool UHotbarPopulation::IsMatchingData(FString _Name)
{
	if (Inventory == PlaceholderInventory)
		GetInventory();
	for (int i = 0; i < Inventory->Contents.Num(); i++) {
		if (!Inventory->Contents[i])
			break;
		if (Inventory->Contents[i]->ItemName.Equals(_Name))
			return true;
	}
	return false;
}

void UHotbarPopulation::GetInventoryData(const int Index, FString &Name, int &Amount)
{
	if (Inventory == PlaceholderInventory)
		GetInventory();
	if (!Inventory->Contents[Index])
		return;
	Name = Inventory->Contents[Index]->ItemName;
	Amount = Inventory->Contents[Index]->Amount;
}

int UHotbarPopulation::GetInventorySize()
{
	if (Inventory == PlaceholderInventory)
		GetInventory();
	return Inventory->Contents.Num();
}

int UHotbarPopulation::GetActiveInventorySlot()
{
	if (Inventory == PlaceholderInventory)
		GetInventory();
	return Inventory->ActiveInventorySlot;
}

void UHotbarPopulation::Giveitem(FString _Name)
{
	if (Inventory == PlaceholderInventory)
		GetInventory();
	Inventory->GiveItem(_Name);
}
