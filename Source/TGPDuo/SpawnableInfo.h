#pragma once

#include "CoreMinimal.h"


class TGPDUO_API SpawnableInfo
{
public:
	//Placeable ID, Placeable Blueprint, Placeable Grid Size
	SpawnableInfo(int _ID, FString _BPN, FVector2D _S);

	~SpawnableInfo();

	int ID;
	FString BluePrintName;
	FVector2D Size;




};

