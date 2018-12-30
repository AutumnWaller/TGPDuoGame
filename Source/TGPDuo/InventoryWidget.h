// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInventory.h"
#include "Classes/Engine/Texture2D.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
class APlayerInventory;
UCLASS()
class TGPDUO_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
private:
public:
	UFUNCTION(BlueprintCallable)
		UTexture2D *GetImageAtIndex(int Index, APlayerInventory *Ref);
	
	
};
