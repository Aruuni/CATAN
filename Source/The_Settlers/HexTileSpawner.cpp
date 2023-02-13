#include "HexTileSpawner.h"
#include "GameManager.h"
#include "HexTile.h"
#include "Thief.h"
#include "NumberTile.h"
#include <Kismet/GameplayStatics.h>


AHexTileSpawner::AHexTileSpawner() {}

// Called when the game starts or when spawned
void AHexTileSpawner::BeginPlay() {
	UWorld* world = GetWorld();
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(world, AGameManager::StaticClass(), foundActors);
	for (AActor* foundActor : foundActors) {
		game = Cast<AGameManager>(foundActor);
	}
	
	gridArray.SetNumZeroed(19);
	AHexTile* origin = GetWorld()->SpawnActor<AHexTile>(DESERT, FVector(FIntPoint(0, 0)), FRotator::ZeroRotator);
	gridArray[0] = origin;
	int8 counter = 0;
	thiefInnit();
	for (int8 hex = 1; hex < 19; ++hex) {
		int randno = rand() % 5;
		TArray<TSubclassOf<AHexTile>> toSpawn = { CLAY, MOUNTAIN, FOREST, PASTURE, FIELD };
		FRotator RotationValue(0.0f, rand() % 5 * 60, 0.0f);
		AHexTile* newTile = GetWorld()->SpawnActor<AHexTile>(toSpawn[randno], catanGrid[hex], RotationValue);
		newTile->settSet();
		newTile->hexSpawner = this;
		newTile->tileType = (EHexTile)(randno + 1);
		gridArray[hex] = newTile;
		++counter;
	}

	TArray<AActor*> foundActors2;
	UGameplayStatics::GetAllActorsOfClass(world, AHexTileSpawner::StaticClass(), foundActors2);
	for (AActor* foundActor : foundActors2) {
		AHexTileSpawner* localgridArray = Cast<AHexTileSpawner>(foundActor);
		gridArray = localgridArray->gridArray;
	}
	TArray<TSubclassOf<ANumberTile>> toSpawn = { TWO, THREE, THREE, FOUR, FOUR, FIVE,  FIVE , SIX, SIX, EIGHT, EIGHT, NINE, NINE, TEN, TEN , ELEVEN, ELEVEN, TWELVE };
	TArray<ENumberTile> number = { ENumberTile::TWO, ENumberTile::THREE, ENumberTile::THREE, ENumberTile::FOUR, ENumberTile::FOUR, ENumberTile::FIVE,  ENumberTile::FIVE , ENumberTile::SIX, ENumberTile::SIX, ENumberTile::EIGHT, ENumberTile::EIGHT, ENumberTile::NINE, ENumberTile::NINE, ENumberTile::TEN, ENumberTile::TEN , ENumberTile::ELEVEN, ENumberTile::ELEVEN, ENumberTile::TWELVE };
	for (int8 numt = 1; numt < 19; ++numt) {
		int random = rand() % toSpawn.Num();
		TSubclassOf<ANumberTile> selected = toSpawn[random];
		toSpawn.RemoveAt(random);
		ENumberTile nr = number[random];
		number.RemoveAt(random);
		ANumberTile* newNumber = GetWorld()->SpawnActor<ANumberTile>(selected, gridArray[numt]->GetActorLocation(), FRotator::ZeroRotator);
		gridArray[numt]->number = (int32)nr;
	}
}

bool AHexTileSpawner::DiceRolled(int32 dice) {
	if (dice != 7) {
		for (int8 i = 0; i < gridArray.Num(); ++i) {
			if (gridArray[i]->number == dice) {
				for (int8 sett = 0; sett < 6; ++sett) {
					ASettlement* cSett = gridArray[i]->settArray[sett];
					AHexTile* hex = gridArray[i];
					if (hex->hasThief) {
						return false;
					}
					cSett->AddResource((EResource)hex->tileType);
				}
			}
		}
	}
	else {
		FString null = FString::Printf(TEXT("Place Thief"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, null);
		game->thiefRound = true;
	}
	return true;
}

void AHexTileSpawner::thiefInnit() {
	GetWorld()->SpawnActor<AThief>(thiefMesh, FVector(0, 0, 0), FRotator::ZeroRotator);
}

void AHexTileSpawner::resetThief() {
	/*or (int8 i = 0; i<gridArray.Num(); ++i) {
		gridArray[i]->hasThief = false;
	}*/
}
