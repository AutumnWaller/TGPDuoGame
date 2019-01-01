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
	uint32 ActiveInventorySlot;
	bool UsingItem;
	const int MaxInventorySlots = 12;
public:	
	TArray<SpawnableInfo*> Contents;
	APlayerInventory();


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsItemPlaceable();

	uint32 GetActiveSlot();

	bool IsUsingItem();

	bool Scroll(int Amount);
	SpawnableInfo* GetItemInSlot(uint32 _Slot) {	return Contents[_Slot]; };
	void GiveItem(SpawnableInfo* _Item);

};