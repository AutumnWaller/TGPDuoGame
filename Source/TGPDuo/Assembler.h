// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Placeable.h"
#include "Assembler.generated.h"

/**
 * 
 */
UCLASS()
class TGPDUO_API AAssembler : public APlaceable
{
	GENERATED_BODY()

public:
	AAssembler();
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;;


};
