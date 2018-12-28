// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldCell.h"

WorldCell::WorldCell()
{
	Terrain = nullptr;
	OreType = 0; //0 = none, 1 = iron, 2 = copper ? iunno
	OreRichness = 0;
	Placeable = nullptr;


}

WorldCell::~WorldCell()
{
	if (Terrain != NULL) {
		delete Terrain;
	}
	if (Placeable != NULL) {
		delete Placeable;
	}
}
