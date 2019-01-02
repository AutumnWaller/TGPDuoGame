// Fill out your copyright notice in the Description page of Project Settings.

#include "HotbarPopulation.h"
void UHotbarPopulation::Initialise()
{ 
	Inventory = GetWorld()->SpawnActor<APlayerInventory>();
}



bool UHotbarPopulation::IsMatchingData(FString _Name)
{
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
	Name = Inventory->Contents[Index]->ItemName;
	Amount = Inventory->Contents[Index]->Amount;
}

int UHotbarPopulation::GetInventorySize()
{
	return Inventory->Contents.Num();
}
