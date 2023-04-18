#include "HexTileSpawner.h"
#include "GameManager.h"
#include "HexTile.h"
#include "NumberTile.h"
#include <Kismet/GameplayStatics.h>

AHexTileSpawner::AHexTileSpawner() {}
// REFACTOR LATER LOW PRIORITY
// Called when the game starts or when spawned
void AHexTileSpawner::BeginPlay() {
	this->hexManager = this;
	gridArray.SetNumZeroed(19);
	AHexTile* origin = GetWorld()->SpawnActor<AHexTile>(DESERT, FVector(FIntPoint(0, 0)), FRotator::ZeroRotator);
	gridArray[0] = origin;
	int8 counter = rand() % 5;
	for (int8 hex = 1; hex < 19; ++hex) {
		TArray<TSubclassOf<AHexTile>> toSpawn = { CLAY, MOUNTAIN, FOREST, PASTURE, FIELD };
		FRotator RotationValue(0.0f, rand() % 5 * 60, 0.0f);
		AHexTile* newTile = GetWorld()->SpawnActor<AHexTile>(toSpawn[counter%5], catanGrid[hex], RotationValue);
		newTile->settSet();
		newTile->tileType = (EHexTile)(counter % 5);
		gridArray[hex] = newTile;
		++counter;
	}
	
	TArray<TSubclassOf<ANumberTile>> toSpawn = { TWO, THREE, THREE, FOUR, FOUR, FIVE,  FIVE , SIX, SIX, EIGHT, EIGHT, NINE, NINE, TEN, TEN , ELEVEN, ELEVEN, TWELVE };
	TArray<ENumberTile> number = { ENumberTile::TWO, ENumberTile::THREE, ENumberTile::THREE, ENumberTile::FOUR, ENumberTile::FOUR, ENumberTile::FIVE,  ENumberTile::FIVE , ENumberTile::SIX, ENumberTile::SIX, ENumberTile::EIGHT, ENumberTile::EIGHT, ENumberTile::NINE, ENumberTile::NINE, ENumberTile::TEN, ENumberTile::TEN , ENumberTile::ELEVEN, ENumberTile::ELEVEN, ENumberTile::TWELVE };
	for (int8 numt = 1; numt < 19; ++numt) {
		int random = rand() % toSpawn.Num();
		TSubclassOf<ANumberTile> selected = toSpawn[random];
		toSpawn.RemoveAt(random);
		ENumberTile nt = number[random];
		number.RemoveAt(random);
		ANumberTile* newNumber = GetWorld()->SpawnActor<ANumberTile>(selected, gridArray[numt]->GetActorLocation(), FRotator::ZeroRotator);
		gridArray[numt]->number = (int32)nt;
	}
}

bool AHexTileSpawner::DiceRolled(int32 dice) {	
	if (dice == 7) { return true; }
	for (int8 hex = 0; hex < gridArray.Num(); ++hex) {
		if (gridArray[hex]->number == dice) {
			if (gridArray[hex]->hasThief) { continue; }
			for (int8 sett = 0; sett < 6; ++sett) {
				gridArray[hex]->settArray[sett]->AddResource((EResource)gridArray[hex]->tileType);
			}
		}
	}
	return false;
}

AHexTile* AHexTileSpawner::GetRandomTile() {
	return gridArray[rand() % gridArray.Num()];
}
