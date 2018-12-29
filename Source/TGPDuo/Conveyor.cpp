// Fill out your copyright notice in the Description page of Project Settings.

#include "Conveyor.h"

UPROPERTY(EditAnywhere, Category = "Mesh")
	UMeshComponent *DefaultComponent;
UPROPERTY(BlueprintReadWrite, Category = "Collision")
	UBoxComponent *MovementBox;

AConveyor::AConveyor()
{
	ConveyorType = 0; //slow/medium/fast
	ConveyorAngle = 0; //straight, left, right.
	ConveyorJoin = 0; //single, start, middle, end
	Size = FVector(1, 1, 0);



	DefaultComponent = CreateDefaultSubobject<UMeshComponent>(TEXT("RootComponent"));

	
	RootComponent = DefaultComponent;
	DefaultComponent->RegisterComponent();

	MovementBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MovementBox"));

	//Maybe an issue with the naming?

	MovementBox->SetRelativeLocation(FVector(0, 0, 50));
	MovementBox->SetWorldRotation(FQuat(0, 0, -90, 0));
	MovementBox->SetRelativeScale3D(FVector(1.75f, 1.5f, 0.75f));
	MovementBox->RegisterComponent();
}

void AConveyor::Update(float DeltaTime) {
	//Push(DeltaTime);
}

void AConveyor::Tick(float DeltaTime)
{
	Push(DeltaTime); 
}

void AConveyor::Push(float DeltaTime)
{
	TArray<AActor*> actorArr;
	MovementBox->GetOverlappingActors(actorArr, TSubclassOf<ACharacter>());

	for(int i = 0; i < actorArr.Num(); i++)
	{
		actorArr[i]->AddActorWorldOffset(GetActorForwardVector() * Speed * DeltaTime);
	}
	//TODO: Make box overlap the mesh so that things will fall off at the end
	//Bug - Only one conveyor works at a time
}

void AConveyor::SetSpeed(float _Speed)
{
	Speed = _Speed;
}

//TODO: make conveyor change its active meshes based on its type, angle and neighbouring blocks
