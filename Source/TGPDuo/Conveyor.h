// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Placeable.h"
#include "Classes/Components/BoxComponent.h"
#include "Classes/GameFramework/Character.h"
#include "Classes/Components/MeshComponent.h"
#include "Conveyor.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ConveyorType : uint8 {
	SLOW,
	MEDIUM,
	FAST
};
UENUM(BlueprintType)
enum class ConveyorAngle : uint8 {
	STRAIGHT,
	LEFT,
	RIGHT
};



UENUM(BlueprintType)
enum class ConveyorJoin : uint8 {
	SINGLE,
	START,
	MIDDLE,
	END
};

UCLASS()
class TGPDUO_API AConveyor : public APlaceable
{
	GENERATED_BODY()
private:
	float Speed = 80;
	UMeshComponent *DefaultComponent;
	UBoxComponent *MovementBox;
	ConveyorType Type = ConveyorType::SLOW;
	ConveyorAngle Angle = ConveyorAngle::STRAIGHT;
	ConveyorJoin Join = ConveyorJoin::SINGLE;
public:

	AConveyor();

	virtual void Update(float DeltaTime) override;
	
	void SetType(ConveyorType _Type);
	void SetAngle(ConveyorAngle _Angle);
	void SetJoin(ConveyorJoin _Join);
	void Set(ConveyorType _Type = ConveyorType::SLOW, ConveyorAngle _Angle = ConveyorAngle::STRAIGHT, ConveyorJoin _Join = ConveyorJoin::SINGLE);
	UFUNCTION(BlueprintCallable)
		ConveyorType GetType() { return Type; }
	UFUNCTION(BlueprintCallable)
		ConveyorAngle GetAngle() { return Angle; }
	UFUNCTION(BlueprintCallable)
		ConveyorJoin GetJoin() { return Join; }
};
