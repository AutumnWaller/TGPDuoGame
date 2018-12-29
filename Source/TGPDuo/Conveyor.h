// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Placeable.h"
#include "Conveyor.generated.h"

/**
 * 
 */
UCLASS()
class TGPDUO_API AConveyor : public APlaceable
{
	GENERATED_BODY()
public:
	int ConveyorType;
	int ConveyorAngle;
	int ConveyorJoin;
	AConveyor();

	void Update(float DeltaTime) override;
	
	
};
