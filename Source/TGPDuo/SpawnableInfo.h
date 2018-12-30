#pragma once

#include "CoreMinimal.h"

class TGPDUO_API SpawnableInfo
{
public:
	//Placeable ID, Placeable Blueprint, Placeable Grid Size
	SpawnableInfo(int _ID, FString _ItemName, FString _BPN, FVector2D _S, int _MaxStackSize);

	~SpawnableInfo();

	int ID;
	FString ItemName;
	FString BluePrintName;
	FString ImagePath;
	FVector2D Size;
	int MaxStackSize;




};

static class TGPDUO_API Spawnables {
public:
	//Moved here so it's accessible anywhere and all together
	static TArray<SpawnableInfo> AvailableSpawnables;

	static SpawnableInfo* GetSpawnable(int _ID) { return new SpawnableInfo(AvailableSpawnables[_ID]); };
	static SpawnableInfo* GetSpawnable(FString _Name);
	
};
