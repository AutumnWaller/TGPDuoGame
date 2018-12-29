// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TGPDuoCharacter.h"
#include "TGPDuoProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "PlayerInventory.h"
#include "Placeable.h"
#include "WorldManager.h"
#include "Engine.h"

//DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);


//////////////////////////////////////////////////////////////////////////
// ATGPDuoCharacter

ATGPDuoCharacter::ATGPDuoCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));


	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 0.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	//custom stuff:
	WorldManager = nullptr;

}

void ATGPDuoCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = Instigator;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Inventory = GetWorld()->SpawnActor<APlayerInventory>(Location, Rotation, SpawnInfo);

	
	GetWorldManager();
	

}

void ATGPDuoCharacter::GetWorldManager() {
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWorldManager::StaticClass(), FoundActors);
	if (FoundActors.Num() == 1) {
		WorldManager = Cast<AWorldManager>(FoundActors[0]);
	}
	else {
		//no worldmanager found, make one?
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void ATGPDuoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATGPDuoCharacter::OnFire);

	//Bind context menu event
	PlayerInputComponent->BindAction("ContextMenu", IE_Pressed, this, &ATGPDuoCharacter::OnContextMenu);


	PlayerInputComponent->BindAxis("MouseScroll", this, &ATGPDuoCharacter::OnScroll);

	// Enable touchscreen input
	//EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ATGPDuoCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ATGPDuoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATGPDuoCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ATGPDuoCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ATGPDuoCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("RotateCW", IE_Pressed, this, &ATGPDuoCharacter::RotateClockWise);
	PlayerInputComponent->BindAction("RotateCCW", IE_Pressed, this, &ATGPDuoCharacter::RotateCounterClockWise);


}

void ATGPDuoCharacter::RotateClockWise()
{
	if (Inventory->GetActiveSlot() != 0) {
		if (InventoryItemRotation == 3) {
			InventoryItemRotation = 0;
		}
		else {
			InventoryItemRotation += 1;
		}
		RotationAnimation -= 1;
		rotating = true;
	}
}

void ATGPDuoCharacter::RotateCounterClockWise()
{
	if (Inventory->GetActiveSlot() != 0) {
		if (InventoryItemRotation == 0) {
			InventoryItemRotation = 3;
		}
		else {
			InventoryItemRotation -= 1;
		}
		RotationAnimation += 1;
		rotating = true;
	}
}

void ATGPDuoCharacter::OnFire()
{

	if (Inventory->GetActiveSlot() != 0) { //first inventory slot is locked to gun for now
		FVector CursorPosition;
		if (Inventory->GetActiveSlot() == 1) {
			//Wrench
			AActor * ObservedObject = nullptr;
			FVector ObjectLocalHit;
			bool lookingAtAnything = LookScanForObject(ObservedObject, CursorPosition, ObjectLocalHit);
			if (lookingAtAnything) {
				APlaceable * Selected = Cast<APlaceable>(ObservedObject);
				if (Selected != nullptr) {
					//object is definitely a placeable.

				}
			}

		}
		else if (Inventory->GetActiveSlot() == 2) {
			//Conveyor Belt
			bool lookingAtAnything = LookScan(CursorPosition);
			FVector GridPosition = CursorPosition / WorldManager->GetGridSize();
			//check occupacy of cell at position
			if (WorldManager != nullptr) {
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Trying to place Conveyor"));
				bool success = WorldManager->AddPlaceable(GridPosition.X, GridPosition.Y, 0, InventoryItemRotation);



				//TODO THIS SHIT





				/*
				WorldCell * Cell;
				if (WorldManager->GetCell(Cell, GridPosition.X, GridPosition.Y) == true) {
					if (GEngine) {
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Space Within World"));
					}
					if (Cell->Placeable==nullptr) {
						if (GEngine) {
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Space unoccupied"));
						}


					}
					else {
						if (GEngine) {
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Space occupied!"));
						}
					}
				}
				else {
					if (GEngine) {
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("cell not within world"));
					}
				}*/
			}
		}
		else if (Inventory->GetActiveSlot() == 3) {
			//Assembler
			bool lookingAtAnything = LookScan(CursorPosition);
			FVector GridPosition = CursorPosition / WorldManager->GetGridSize();
			//check occupacy of cell at position
			if (WorldManager != nullptr) {
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Trying to place Assembler"));
				bool success = WorldManager->AddPlaceable(GridPosition.X, GridPosition.Y, 1, InventoryItemRotation);
			}

		}
	}

}




	//InventoryItem Equipped = Iventory.GetItem[ActiveInventorySlot]
	//if (Equipped)
	//{
		//do stuff
	//}

	/*
	//////////////////DEFAULT PROJECTILE STUFF:
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<AEntityPlacementDemoProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AEntityPlacementDemoProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PEW"));
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
	*/

void ATGPDuoCharacter::OnScroll(float AxisValue)
{
	if (AxisValue != 0.0f) {
		bool HasScrolled = Inventory->Scroll(AxisValue);
		if (HasScrolled) {
			UpdateEquipped();
		}
	}
}


void ATGPDuoCharacter::UpdateEquipped() {

	//TEMPORARY
	if (Inventory->GetActiveSlot() == 0) {
		//show gun:
		if (bUsingMotionControllers)
		{
			VR_Gun->SetHiddenInGame(false, true);
			Mesh1P->SetHiddenInGame(true, true);
		}
		else
		{
			VR_Gun->SetHiddenInGame(true, true);
			Mesh1P->SetHiddenInGame(false, true);
		}
	}
	else {
		if (bUsingMotionControllers)
		{
			VR_Gun->SetHiddenInGame(true, true);
			Mesh1P->SetHiddenInGame(true, true);
		}
		else
		{
			VR_Gun->SetHiddenInGame(true, true);
			Mesh1P->SetHiddenInGame(true, true);
		}
	}
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Switched to Inventory slot " + FString::FromInt(Inventory->GetActiveSlot())));
}

void ATGPDuoCharacter::OnContextMenu()
{

}

void ATGPDuoCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}


//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void ATGPDuoCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void ATGPDuoCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ATGPDuoCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ATGPDuoCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATGPDuoCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATGPDuoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Inventory->GetActiveSlot() != 0) { //first inventory slot is locked to gun for now
		FVector CursorPosition;
		if (Inventory->GetActiveSlot() == 1) {
			//Wrench
			AActor * ObservedObject = nullptr;
			FVector ObjectLocalHit;
			bool lookingAtAnything = LookScanForObject(ObservedObject, CursorPosition, ObjectLocalHit);
		}
		else {
			if (rotating) {
				float rotateamount = 1 * DeltaTime;
				//RotationAnimation wants to be 0
				if (RotationAnimation < rotateamount && RotationAnimation > -rotateamount) { //is it really close to 0?
					//snap rotation and stop;
					RotationAnimation = 0;
					rotating = false;
				}
				else {
					//is mid-rotation, so animate
					if (RotationAnimation > 2) { // more than 180
						RotationAnimation -= 4;
					}
					if (RotationAnimation < -2) { //less than -18
						RotationAnimation += 4;
					}
					if (RotationAnimation > 0) {
						RotationAnimation -= rotateamount;
					}
					else {
						RotationAnimation += rotateamount;
					}
				}

				float angle = (InventoryItemRotation-RotationAnimation)*90;


				//TODO: set inventory ghost angle here;
			}
			if (Inventory->GetActiveSlot() == 2) {
				//Conveyor Belt
				bool lookingAtAnything = LookScan(CursorPosition);
				//check occupacy of cell at position
			}
			else if (Inventory->GetActiveSlot() == 3) {
				//Assembler
				bool lookingAtAnything = LookScan(CursorPosition);
				//check occupacy of cell at position+size
			}
		}
	}
}


bool ATGPDuoCharacter::LookScan(FVector & CursorPosition)
{
	AActor * Ob = nullptr;
	FVector F;
	return (this->LookScanForObject(Ob, CursorPosition, F));
}

bool ATGPDuoCharacter::LookScanForObject(AActor *& Object, FVector &CursorPosition, FVector &ObjectLocalHit)
{
	APlayerCameraManager * CM;
	if (GEngine) {
		CM = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;

		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;
		//RV_TraceParams.AddIgnoredActor(CM->actor);
		const FName TraceTag("MyTraceTag");
		RV_TraceParams.TraceTag = TraceTag;
		//RV_TraceParams.bIgnoreBlocks = true;
		//GetWorld()->DebugDrawTraceTag = TraceTag;
		//Re-initialize hit info
		//FHitResult RV_Hit(ForceInit);
		TArray<FHitResult> RV_Hit;
		FVector Start = CM->GetCameraLocation();
		FVector Forward = CM->GetActorForwardVector();
		FVector End = Start + Forward * 2000.0f; //1 meter = 100
		Start = Start + Forward * 10; //slightly in front of eye, not ON eye?
		//call GetWorld() from within an actor extending class
		//TODO: limit to particular objects looking for?

		//I don't know how to do that easily.

		//bool Success = GetWorld()->LineTraceSingleByChannel(RV_Hit, Start, End, ECC_Pawn, RV_TraceParams);
		//bool Success = GetWorld()->LineTraceMultiByObjectType(RV_Hit, Start, End, ECC_Pawn, RV_TraceParams);
		bool Success = GetWorld()->LineTraceMultiByChannel(RV_Hit, Start, End, ECC_Pawn, RV_TraceParams);
		if (Success) {
			FHitResult HitActor;
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Success")); //spams, be careful.
			bool ValidHit = false;
			if (RV_Hit.Num() > 0) {
				for (int i = 0; i < RV_Hit.Num(); i++) {
					HitActor = RV_Hit[i];
					Object = HitActor.GetActor();
					if (Object->IsA(APlaceable::StaticClass())) {
						//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hit Placeable"));
						ValidHit = true;
						break;
					}
					else if (Object->GetName()=="Floor") {
						//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hit Floor"));
						ValidHit = true;
						break;
					}

				}
			}

			int GridSize = WorldManager->GetGridSize();
			if (ValidHit) {
				FVector ActorPosition = Object->GetActorTransform().GetLocation();
				if (Object->IsA(APlaceable::StaticClass())) {
					APlaceable* Placeable = Cast<APlaceable>(Object);
					FVector Size = Placeable->Size;
					//if an axis in the size is not an even number, then the position on that axis will be off by 50, so correct for that
					FVector Offset = FVector((FMath::Fmod(Size.X, 2) == 0) ? -GridSize/2 : 0, (FMath::Fmod(Size.Y, 2) == 0) ? -GridSize/2 : 0, 0);
					ActorPosition = ActorPosition + Offset;
				}
				else {
					if (Object->GetName() == "Floor") {
						ActorPosition = FVector(0, 0, 0);
					}
				}

				//these may need to be based on a simplified hitbox model or something, so that its easier to position blocks next to each other
				FVector RayHitPos = HitActor.ImpactPoint - Forward * 5.0f; //step backfrom the surface a little bit?



				
				FVector RelativeOffset = RayHitPos - ActorPosition;
				RelativeOffset.X = FMath::DivideAndRoundNearest(int(RelativeOffset.X), GridSize)*GridSize;
				RelativeOffset.Y = FMath::DivideAndRoundNearest(int(RelativeOffset.Y), GridSize)*GridSize;
				RelativeOffset.Z = 0; //floor.
				ObjectLocalHit = RelativeOffset; // will have to work this out properly later
				FVector RelativePosition = ActorPosition + RelativeOffset;

				FVector ActualPosition = FVector(FMath::DivideAndRoundNearest(int(RelativePosition.X), GridSize)*GridSize, FMath::DivideAndRoundNearest(int(RelativePosition.Y), GridSize)*GridSize, 0.f);
				CursorPosition = ActualPosition;

				UWorld* World = GetWorld();

				//relative cursor:
				DrawDebugLine(World, RelativePosition + FVector(-GridSize / 2, -GridSize / 2, 15), RelativePosition + FVector(GridSize / 2, -GridSize / 2, 15), FColor(255, 0, 255), false, -1.f, 0, 5.f);
				DrawDebugLine(World, RelativePosition + FVector(-GridSize / 2, GridSize / 2, 15), RelativePosition + FVector(GridSize / 2, GridSize / 2, 15), FColor(255, 0, 255), false, -1.f, 0, 5.f);
				DrawDebugLine(World, RelativePosition + FVector(-GridSize / 2, -GridSize / 2, 15), RelativePosition + FVector(-GridSize / 2, GridSize / 2, 15), FColor(255, 0, 255), false, -1.f, 0, 5.f);
				DrawDebugLine(World, RelativePosition + FVector(GridSize / 2, -GridSize / 2, 15), RelativePosition + FVector(GridSize / 2, GridSize / 2, 15), FColor(255, 0, 255), false, -1.f, 0, 5.f);


				//draw a square to show me where the cursor is:
				DrawDebugLine(World, ActualPosition + FVector(-GridSize / 2, -GridSize / 2, 5), ActualPosition + FVector(GridSize / 2, -GridSize / 2, 5), FColor(255, 0, 0), false, -1.f, 0, 5.f);
				DrawDebugLine(World, ActualPosition + FVector(-GridSize / 2, GridSize / 2, 5), ActualPosition + FVector(GridSize / 2, GridSize / 2, 5), FColor(255, 0, 0), false, -1.f, 0, 5.f);
				DrawDebugLine(World, ActualPosition + FVector(-GridSize / 2, -GridSize / 2, 5), ActualPosition + FVector(-GridSize / 2, GridSize / 2, 5), FColor(0, 255, 0), false, -1.f, 0, 5.f);
				DrawDebugLine(World, ActualPosition + FVector(GridSize / 2, -GridSize / 2, 5), ActualPosition + FVector(GridSize / 2, GridSize / 2, 5), FColor(0, 255, 0), false, -1.f, 0, 5.f);

				/*DrawDebugSphere(
					GetWorld(),
					NewPosition,
					24,
					32,
					FColor(255, 0, 255)
				);*/
				
				return true;
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("No valid Actors hit"));
			}
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Cast Fail"));
			return false;
		}
	}
	return false;
}