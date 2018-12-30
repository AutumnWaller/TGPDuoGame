// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidget.h"

UTexture2D* UInventoryWidget::GetImageAtIndex(int Index, APlayerInventory *Ref){
	const TCHAR *Name = *Ref->Contents[Index]->ItemName;
	const TCHAR *Path = *Ref->Contents[Index]->ImagePath;
	UTexture2D* Texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, (Path)));
	return Texture;
}

