// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnableInfo.h"

#include "PlayerInventory.generated.h"

UCLASS()
class TGPDUO_API APlayerInventory : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool UsingItem;
	const int MaxInventorySlots = 6;
public:	
	TArray<SpawnableInfo*> Contents;
	APlayerInventory();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsItemPlaceable();

	bool IsUsingItem();

	bool Scroll(int Amount);
	uint32 ActiveInventorySlot;

	SpawnableInfo* GetItemInSlot(uint32 _Slot) {	return Contents[_Slot]; };
	SpawnableInfo* GetSpawnableByName(FString _Name);
	bool GiveItem(SpawnableInfo* _Item, UINT32 amount);
	bool GiveItem(SpawnableInfo* _Item);
	bool GiveItem(FString _Name, UINT32 amount);
	bool GiveItem(FString _Name);
	bool RemoveItem(int Index, UINT32 amount);
	bool RemoveItem(int Index);
	bool RemoveItem(FString _Name, UINT32 amount);
	bool RemoveItem(FString _Name);
};