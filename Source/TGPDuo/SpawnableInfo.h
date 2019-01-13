#pragma once

#include "CoreMinimal.h"

class TGPDUO_API SpawnableInfo
{
public:
	//Placeable ID, Placeable Blueprint, Placeable Grid Size
	SpawnableInfo(UINT32 _ID, FString _ItemName, FString _BPN, FVector2D _S, UINT32 _Amount, UINT32 _MaxStackSize);
	~SpawnableInfo();

	UINT32 ID;
	FString ItemName;
	FString BluePrintName;
	FVector2D Size;
	UINT32 MaxStackSize;
	UINT32 Amount;
};

static class TGPDUO_API Spawnables {
public:
	//Moved here so it's accessible anywhere and all together
	static TArray<SpawnableInfo> AvailableSpawnables;

	static SpawnableInfo* GetSpawnable(UINT32 _ID) { return new SpawnableInfo(AvailableSpawnables[_ID]); };
	static SpawnableInfo* GetSpawnable(FString _Name);
	
};
