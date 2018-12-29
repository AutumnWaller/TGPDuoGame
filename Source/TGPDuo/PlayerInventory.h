// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "PlayerInventory.generated.h"


UCLASS()
class TGPDUO_API APlayerInventory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerInventory();

	

	
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	TArray<class AInventoryItem*> Contents;
	uint32 ActiveInventorySlot;
	bool UsingItem;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsItemPlaceable();

	uint32 GetActiveSlot();

	bool IsUsingItem();

	bool Scroll(int Amount);
	
};
