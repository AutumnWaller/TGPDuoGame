// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemDefinition.h"

ItemDefinition::ItemDefinition(int _ID, bool _Placeable, int _MaxStackSize)
{
	ID = _ID;
	Placeable = _Placeable;
	MaxStackSize = _MaxStackSize;
}

ItemDefinition::~ItemDefinition()
{
}
