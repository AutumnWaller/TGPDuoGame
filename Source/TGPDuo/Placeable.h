// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "Placeable.generated.h"

UCLASS()
class TGPDUO_API APlaceable : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlaceable();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BlockData)
		FVector Size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BlockData)
		FVector Position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BlockData)
		int Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BlockData)
		bool Placed;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Update(float DeltaTime);

};
