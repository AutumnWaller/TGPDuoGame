// Fill out your copyright notice in the Description page of Project Settings.

#include "Conveyor.h"

AConveyor::AConveyor()
{
	ConveyorType = 0; //slow/medium/fast
	ConveyorAngle = 0; //straight, left, right.
	ConveyorJoin = 0; //single, start, middle, end
	Size = FVector(1, 1, 0);
}

void AConveyor::Update(float DeltaTime) {
	//placeable tick
}

//TODO: make conveyor change its active meshes based on its type, angle and neighbouring blocks