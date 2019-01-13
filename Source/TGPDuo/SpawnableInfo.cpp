#include "SpawnableInfo.h"



SpawnableInfo::SpawnableInfo(UINT32 _ID, FString _ItemName, FString _BPN, FVector2D _S, UINT32 _Amount = 1, UINT32 _MaxStackSize = 99)
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
	for (UINT32 i = 0; i < UINT32(AvailableSpawnables.Num()); i++) {
		if (AvailableSpawnables[i].ItemName == _Name)
			return new SpawnableInfo(AvailableSpawnables[i]);
	}
	return nullptr;
}

TArray<SpawnableInfo> Spawnables::AvailableSpawnables = {
	{ SpawnableInfo(0, "Empty", "", FVector2D(0, 0), 0, 0)},
	{ SpawnableInfo(1, "Conveyor", "Blueprint'/Game/Game/Placeables/Conveyor/BP_Conveyor.BP_Conveyor_C'", FVector2D(1, 1), 1, 99)},
	{ SpawnableInfo(2, "Assembler", "Blueprint'/Game/Game/Placeables/Assembler/BP_Assembler.BP_Assembler_C'", FVector2D(3, 3), 1, 99) }
};