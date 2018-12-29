#include "SpawnableInfo.h"



SpawnableInfo::SpawnableInfo(int _ID, FString _BPN, FVector2D _S)
{
	ID = _ID;
	BluePrintName = _BPN;
	Size = _S;
}


SpawnableInfo::~SpawnableInfo()
{
}
