// Fill out your copyright notice in the Description page of Project Settings.

#include "Placeable.h"
#include "Engine.h"

// Sets default values
APlaceable::APlaceable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Size = FVector(1, 1, 0);

	Position = FVector(0, 0, 0); //grid position?
	int Rotation = 0;
	bool Placed = false;


}

// Called when the game starts or when spawned
void APlaceable::BeginPlay()
{
	Super::BeginPlay();
	Position = this->GetTransform().GetLocation();

}

// Called every frame
void APlaceable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*DrawDebugSphere(
		GetWorld(),
		Position+FVector(0,0,200),
		24,
		32,
		FColor(255, 0, 255)
	);*/
	if (Placed) {
		this->Update(DeltaTime);
	}
}

void APlaceable::Update(float DeltaTime)
{
}


//TODO: make meshes transparent when in placement mode?