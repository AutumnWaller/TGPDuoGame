// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Placeable.h"
#include "Classes/Components/BoxComponent.h"
#include "Classes/GameFramework/Character.h"
#include "Classes/Components/MeshComponent.h"
#include "Conveyor.generated.h"

/**
 * 
 */
UCLASS()
class TGPDUO_API AConveyor : public APlaceable
{
	GENERATED_BODY()
private:
	float Speed = 80;
public:
	int ConveyorType;
	int ConveyorAngle;
	int ConveyorJoin;
	AConveyor();

	virtual void Update(float DeltaTime) override;
	virtual void Tick(float DeltaTime) override;
	void Push(float DeltaTime);
	float GetSpeed() { return Speed; };
	void SetSpeed(float _Speed);
};
