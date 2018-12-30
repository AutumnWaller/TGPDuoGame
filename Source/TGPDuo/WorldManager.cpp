// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldManager.h"



// Sets default values
AWorldManager::AWorldManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Seed = 0;
	Width = 20;
	Height = 20;
	GridSize = 100;



	//CREATE LIST OF PLACEABLE STUFF HERE, Should be loaded from external file at some point probably.
	//Moved the list into its own class inside of SpawnableInfo.h for accessibility

}

// Called when the game starts or when spawned
void AWorldManager::BeginPlay()
{
	Super::BeginPlay();
	GenerateWorld(false, Width, Height);

}

// Called every frame
void AWorldManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AWorldManager::GetCell(WorldCell *& Cell, int x, int y)
{
	if (Width > 0 && Height > 0) {
		if (x >= 0 && x < Width && y >= 0 && y < Height) {
			if (levelMap.IsValidIndex(x)) {
				TArray<WorldCell*> Column = levelMap[x];
				if (Column.IsValidIndex(y)) {
					Cell = Column[y];
					return true;
				}
			}
		}
		else {
			//OUTSIDE BOUNDS OF WORLD
		}
	}
	else {
		//WORLD HAS NOT YET BEEN DEFINED
	}
	return false;
}

void AWorldManager::GenerateWorld(bool Populate, int width, int height)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("World Generating"));

	Width = width;
	Height = height;
	//generate empty table:
	for (int x = 0; x < width; x++) {
		TArray <WorldCell*> Column;
		for (int y = 0; y < height; y++) {
			WorldCell * NewCell = new WorldCell();
			if (Column.IsValidIndex(y)) {
				Column.RemoveAt(y); //fuck it I don't know what to do in the event of re-initialisation
				Column.Add(NewCell);
			}
			else {
				Column.Add(NewCell);
			}
		}
		levelMap.Add(Column);

	}

	//populate: whether to fill world with stuff or just leave empty
	if (Populate) {

	}
	else {






	}
}

bool AWorldManager::AddPlaceable(int x, int y, int ID, int Rotation)
{
	if (CanBePlaced(x, y, ID)) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Can be placed"));
		FVector2D SpawnSize = SpawnableInformation[ID]->Size;
		FVector Offset = FVector(0, 0, 0);
		//remember that the actual center position of placeables will be off by 0.5 if they are even on either axis
		//the actual grid position will similarly be off by -0.5, it's easier to think grid position is "middle bottom left".
		//so actual mesh position is "middle top right", since it's gonna be in the positive direction.
		if (FMath::Fmod(SpawnSize.X, 2) == 0) {
			Offset.X = 0.5; 
		}
		if (FMath::Fmod(SpawnSize.Y, 2) == 0) {
			Offset.Y = 0.5;
		}

		FString BPName = SpawnableInformation[ID]->BluePrintName;

		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Blueprint Name:"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, BPName);
		}

		
		UClass* PlaceableBlueprint = LoadObject<UClass>(this, *BPName);
		if (PlaceableBlueprint == nullptr) {
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Blueprint not found"));
		}
		FActorSpawnParameters SpawnInfo;
		FVector Location((x+Offset.X)*GridSize, (y+Offset.Y)*GridSize, 0.0f);
		FRotator Rotation(0.0f, 0.0f, 90*Rotation);

		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = Instigator;

		APlaceable * P = GetWorld()->SpawnActor<APlaceable>(PlaceableBlueprint, Location, Rotation, SpawnInfo);
		FVector2D ScanStart = FVector2D(0, 0);
		FVector2D ScanEnd = FVector2D(0, 0);
		if (FMath::Fmod(SpawnSize.X, 2) == 0) {
			ScanStart.X = -SpawnSize.X / 2 + 1;
			ScanEnd.X = SpawnSize.X / 2;
		}
		else {
			ScanStart.X = -(SpawnSize.X + 1) / 2 + 1;
			ScanEnd.X = (SpawnSize.X + 1) / 2 - 1;
		}
		if (FMath::Fmod(SpawnSize.Y, 2) == 0) {
			ScanStart.Y = -SpawnSize.Y / 2 + 1;
			ScanEnd.Y = SpawnSize.Y / 2;
		}
		else {
			ScanStart.Y = -(SpawnSize.Y + 1) / 2 + 1;
			ScanEnd.Y = (SpawnSize.Y + 1) / 2 - 1;
		}
		for (int X = ScanStart.X; X <= ScanEnd.X; X++)
		{
			for (int Y = ScanStart.Y; Y <= ScanEnd.Y; Y++)
			{
				WorldCell * Cell;
				bool valid = GetCell(Cell, x + X, y + Y);
				Cell->Placeable = P;
			}
		}
	}
	return false;
}
bool AWorldManager::CanBePlaced(int x, int y, int ID)
{
	if (SpawnableInformation.IsValidIndex(ID)) //TODO: replace with actual ID lookup when it matters
	{
		FVector2D SpawnSize = SpawnableInformation[ID]->Size;
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("SIZE (" + FString::FromInt(SpawnSize.X) + " |" + FString::FromInt(SpawnSize.X) + ")"));
		}
		FVector2D ScanStart = FVector2D(0, 0);
		FVector2D ScanEnd = FVector2D(0, 0);
		if (FMath::Fmod(SpawnSize.X, 2) == 0) {
			ScanStart.X = -SpawnSize.X / 2 + 1;
			ScanEnd.X = SpawnSize.X / 2;
		}
		else {
			ScanStart.X = -(SpawnSize.X + 1) / 2 + 1;
			ScanEnd.X = (SpawnSize.X + 1) / 2 - 1;
		}
		if (FMath::Fmod(SpawnSize.Y, 2) == 0) {
			ScanStart.Y = -SpawnSize.Y / 2 + 1;
			ScanEnd.Y = SpawnSize.Y / 2;
		}
		else {
			ScanStart.Y = -(SpawnSize.Y + 1) / 2 + 1;
			ScanEnd.Y = (SpawnSize.Y + 1) / 2 - 1;
		}

		for (int X = ScanStart.X; X <= ScanEnd.X; X++)
		{
			for (int Y = ScanStart.Y; Y <= ScanEnd.Y; Y++)
			{
				WorldCell * Cell;
				bool valid = GetCell(Cell, x + X, y + Y);
				if (valid == false) {
					//cell is out of bounds
					if (GEngine)
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cell is Out of bounds!"));
					return false;
				}
				else {
					//cell in bounds, check occupancy(manually, to save time)
					if (Cell->Placeable != nullptr) {
						//cell has a placeable.
						if (GEngine)
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cell is Occupied!"));
						return false;
					}
				}
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Cell is OK!"));
			}
		}
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("All cells are OK"));
		return true;
	}
	return false;
}


bool AWorldManager::IsCellOccupied(int x, int y)
{
	WorldCell * Cell;
	if (GetCell(Cell, x, y) == true) {
		if (Cell->Placeable != nullptr) {
			return true;
		}
		
	}
	return false;
}

int AWorldManager::GetGridSize()
{
	return GridSize;
}


void AWorldManager::DeleteWorld() {
	/*for (int x = 0; x < Width; x++) {
		for (int y = 0; y < Height; y++) {
		}
	}*/
}
