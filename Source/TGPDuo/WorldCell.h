
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Placeable.h"

/**
 * 
 */
class TGPDUO_API WorldCell
{
public:
	WorldCell();
	~WorldCell();

	AActor* Terrain;
	int OreType; //0 = coal, 1 = iron, 2 = copper ? iunno
	float OreRichness;
	APlaceable* Placeable;

};
