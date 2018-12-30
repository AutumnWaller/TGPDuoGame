// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TGPDUO_API ItemDefinition
{
protected:
	int ID, MaxStackSize;
	bool Placeable;
public:
	ItemDefinition(int _ID, bool _Placeable, int _MaxStackSize);
	~ItemDefinition();
};

class TGPDUO_API ItemList {

};