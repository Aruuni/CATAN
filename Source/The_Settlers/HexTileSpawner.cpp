#include "HexTileSpawner.h"
#include "HexTile.h"
#include "Thief.h"
#include "NumberTile.h"
#include <Kismet/GameplayStatics.h>

AHexTileSpawner::AHexTileSpawner() {}

void AHexTileSpawner::BeginPlay() {
	hexManager = this;
	//an array of all the hex tiles that are spawned is created with 19 empty slots
	gridArray.SetNumZeroed(19);
	// the spawn bucket is set 
	TArray<TSubclassOf<AHexTile>> spawnBucket = hexBucket(19);
	// the hex tiles are spawned one by one in the game world, based on the coordinates in the catan grid array
	for (int8 hex = 0; hex < 19; ++hex) {
		//a random number is generated to select a random hex tile from the spawn bucket
		int32 random = rand() % spawnBucket.Num();
		//the hex tile is spawned at the coordinates of the catan grid array
		AHexTile* newTile = GetWorld()->SpawnActor<AHexTile>(spawnBucket[random], catanGrid[hex], FRotator(0.0f, rand() % 5 * 60, 0.0f));
		//the hex tile is removed from the spawn bucket so it cant be spawned again
		spawnBucket.RemoveAt(random);
		//settSet is called to set the settlement adjacent on the hex tile
		newTile->settSet();
		//the hex tile is added to the grid array
		gridArray[hex] = newTile;
	}
	// number tiles are in a hard coded array 
	TArray<TSubclassOf<ANumberTile>> toSpawn = { TWO, THREE, THREE, FOUR, FOUR, FIVE,  FIVE , SIX, SIX, EIGHT, EIGHT, NINE, NINE, TEN, TEN , ELEVEN, ELEVEN, TWELVE };
	// these enums are used to extract an integer from the number tile, as the number tile class is simply a mesh
	TArray<ENumberTile> number = { ENumberTile::TWO, ENumberTile::THREE, ENumberTile::THREE, ENumberTile::FOUR, ENumberTile::FOUR, ENumberTile::FIVE,  ENumberTile::FIVE , ENumberTile::SIX, ENumberTile::SIX, ENumberTile::EIGHT, ENumberTile::EIGHT, ENumberTile::NINE, ENumberTile::NINE, ENumberTile::TEN, ENumberTile::TEN , ENumberTile::ELEVEN, ENumberTile::ELEVEN, ENumberTile::TWELVE };
	for (int8 numt = 0; numt < 19; ++numt) {
		// the desert hex tile is skipped as it has no number tile
		if (gridArray[numt]->tileType == EHexTile::DESERT) {
			AThief::thief->SetActorLocation(gridArray[numt]->GetActorLocation());
			gridArray[numt]->hasThief = true;
			continue;
		}
		// a random number is generated to select a random number tile from the toSpawn bucket
		int random = rand() % toSpawn.Num();
		TSubclassOf<ANumberTile> selected = toSpawn[random];
		toSpawn.RemoveAt(random);
		ENumberTile nt = number[random];
		number.RemoveAt(random);
		ANumberTile* newNumber = GetWorld()->SpawnActor<ANumberTile>(selected, gridArray[numt]->GetActorLocation(), FRotator::ZeroRotator);
		// the number is set on the hex tile
		gridArray[numt]->number = (int32)nt;
	}
}


// called from GameManager to roll the dice every round, it adds resourced to the players
// dice is a random number between 2 and 12
// returns true if the dice is 7, false otherwise
bool AHexTileSpawner::DiceRolled(int32 dice) {	
	if (dice == 7) { 
		// returns true as resources cant be earned
		return true; 
	}
	for (int8 hex = 0; hex < gridArray.Num(); ++hex) {
		if (gridArray[hex]->number == dice) {
			// ignores the hex if it has a thief on it
			if (gridArray[hex]->hasThief) { 
				continue; 
			}
			// earns the resource if the hex is not blocked
			gridArray[hex]->earnResource();
		}
	}
	return false;
}

// gets a random pointer to a hex tile
// returns the pointer to the hex tile
AHexTile* AHexTileSpawner::GetRandomTile() {
	return gridArray[rand() % gridArray.Num()];
}

TArray<TSubclassOf<AHexTile>> AHexTileSpawner::hexBucket(int32 ammount) {
	TArray<TSubclassOf<AHexTile>>  possible = { CLAY, MOUNTAIN, FOREST, PASTURE, FIELD };
	TArray<TSubclassOf<AHexTile>> bucket;
	int16 count = rand() % 5;
	bucket.Add(DESERT);
	for (int8 hex = 1; hex < ammount; ++hex) {
		bucket.Add(possible[count]);
		count = ++count % 5;
	}
	return bucket;
}

