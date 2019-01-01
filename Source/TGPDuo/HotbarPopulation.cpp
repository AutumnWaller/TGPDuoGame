// Fill out your copyright notice in the Description page of Project Settings.

#include "HotbarPopulation.h"
void UHotbarPopulation::Initialise()
{ 
	Inventory = GetWorld()->SpawnActor<APlayerInventory>();
	FString Test = Inventory->Contents[0]->ItemName;
	//TArray<AActor*> Actors = GetWorld()->PersistentLevel->Actors;
	//for (int i = 0; i < Actors.Num(); i++) {
	//	if (Actors[i]->GetClass() == APlayerInventory::StaticClass())
	//		Inventory = (APlayerInventory*)Actors[i]->GetClass();
	//}
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
