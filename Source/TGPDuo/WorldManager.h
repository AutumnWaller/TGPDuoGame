// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "WorldCell.h"
#include "SpawnableInfo.h"
#include "WorldManager.generated.h"




UCLASS()
class TGPDUO_API AWorldManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetCell(WorldCell *&Cell, int x, int y);

	int Seed;


	bool AddPlaceable(int x, int y, int ID, int Rotation);
	bool CanBePlaced(int x, int y, int ID);

	bool IsCellOccupied(int x, int y);

	int GetGridSize();

private:
	TArray<TArray<WorldCell*>> levelMap;
	void GenerateWorld(bool Populate, int width, int height);



	void DeleteWorld();
	

	int Width;
	int Height;
	int GridSize;

	TArray<SpawnableInfo*> SpawnableInformation;

};
