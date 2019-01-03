#include "SpawnableInfo.h"



SpawnableInfo::SpawnableInfo(int _ID, FString _ItemName, FString _BPN, FVector2D _S, int _Amount = 1, int _MaxStackSize = 99)
{
	ID = _ID;
	ItemName = _ItemName;
	BluePrintName = _BPN;
	Size = _S;
	MaxStackSize = _MaxStackSize;
	Amount = _Amount;
}


SpawnableInfo::~SpawnableInfo()
{
}

SpawnableInfo * Spawnables::GetSpawnable(FString _Name)
{
	for (int i = 0; i < AvailableSpawnables.Num(); i++) {
		if (AvailableSpawnables[i].ItemName == _Name)
			return new SpawnableInfo(AvailableSpawnables[i]);
	}
	return nullptr;
}

TArray<SpawnableInfo> Spawnables::AvailableSpawnables = {
	{ SpawnableInfo(-1, "Empty", "", FVector2D(0, 0), 0, 0)},
	{ SpawnableInfo(0, "Conveyor", "Blueprint'/Game/Game/Placeables/Conveyor/BP_Conveyor.BP_Conveyor'", FVector2D(1, 1), 1, 99)},
	{ SpawnableInfo(1, "Assembler", "Blueprint'/Game/Game/Placeables/Assembler/BP_Assembler.BP_Assembler_C'", FVector2D(3, 3), 1, 99) }
};