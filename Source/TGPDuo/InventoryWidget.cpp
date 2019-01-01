// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryWidget.h"

UTexture2D* UInventoryWidget::GetImageAtIndex(int Index, APlayerInventory *Ref){
	if (Ref->Contents[Index] == NULL)
		return nullptr;
	const TCHAR *Name = *Ref->Contents[Index]->ItemName;
	const TCHAR *Path = *Ref->Contents[Index]->ImagePath;
	UTexture2D* Texture = (UTexture2D*)StaticLoadObject(UTexture2D::StaticClass(), NULL, Path, (Path));
	return Texture;
}