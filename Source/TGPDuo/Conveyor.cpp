// Fill out your copyright notice in the Description page of Project Settings.

#include "Conveyor.h"



AConveyor::AConveyor()
{
	Size = FVector(1, 1, 0);



	DefaultComponent = CreateDefaultSubobject<UMeshComponent>(TEXT("RootComponent"));
	DefaultComponent->RegisterComponent();
	RootComponent = DefaultComponent;
	MovementBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MovementBox"));
	MovementBox->SetupAttachment(DefaultComponent);
	MovementBox->SetRelativeLocation(FVector(0, 0, 50));
	MovementBox->SetWorldRotation(FQuat(0, 0, -90, 0));
	MovementBox->SetRelativeScale3D(FVector(1.5f, 1.5f, 0.75f));
	MovementBox->RegisterComponent();
}

void AConveyor::Update(float DeltaTime) {
	APlaceable::Update(DeltaTime);
	Push(DeltaTime);
}
void AConveyor::Push(float DeltaTime)
{
	TArray<AActor*> actorArr;
	this->MovementBox->GetOverlappingActors(actorArr, TSubclassOf<ACharacter>());

	for(int i = 0; i < actorArr.Num(); i++)
	{
		actorArr[i]->AddActorWorldOffset(GetActorForwardVector() * Speed * DeltaTime);
	}
}

void AConveyor::SetType(ConveyorType _Type)
{
	Type = _Type;
	switch (Type) {
	case ConveyorType::SLOW:
			Speed = 80;
			break;
	case ConveyorType::MEDIUM:
			Speed = 140;
			break;
	case ConveyorType::FAST:
			Speed = 300;
			break;
	default:
		Speed = 0;
		break;
	}
}

void AConveyor::SetAngle(ConveyorAngle _Angle)
{
	Angle = _Angle;
}

void AConveyor::SetJoin(ConveyorJoin _Join)
{
	Join = _Join;
}

void AConveyor::Set(ConveyorType _Type, ConveyorAngle _Angle, ConveyorJoin _Join)
{
	Type = _Type;
	Angle = _Angle;
	Join = _Join;
}

//TODO: make conveyor change its active meshes based on its type, angle and neighbouring blocks
