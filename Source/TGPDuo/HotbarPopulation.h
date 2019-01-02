// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerInventory.h"
#include "SpawnableInfo.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Classes/Engine/Texture2D.h"
#include "HotbarPopulation.generated.h"

/**
 * 
 */
UCLASS()
class TGPDUO_API UHotbarPopulation : public UUserWidget
{
	GENERATED_BODY()
public:
		APlayerInventory* Inventory;
		UFUNCTION(BlueprintCallable)
		void Initialise();
	
		UFUNCTION(BlueprintCallable)
		bool IsMatchingData(FString _Name);

		UFUNCTION(BlueprintCallable)
		void GetInventoryData(const int Index, FString &Name, int &Amount);

		UFUNCTION(BlueprintCallable)
			int GetInventorySize();
	
};
