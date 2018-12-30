// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnableInfo.h"
#include "InventoryItem.generated.h"

UCLASS()
class TGPDUO_API AInventoryItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AInventoryItem();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
