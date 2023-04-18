#include "Thief.h"
#include <Kismet/GameplayStatics.h>

AThief::AThief() {
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	thiefMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bMesh"));
	thiefMesh->SetupAttachment(RootComponent);
}
//remove the lcoc
void AThief::moveThief(FVector loc, AHexTile* newTile) {
	if (!AGameManager::thiefLock) { return; }
	// if the tile is not a resource tile, the thief cannot move there, it was able to move there before and it was kinda funny, but it was not intended.
	if ((int32)newTile->tileType > 5) { return; }
	//Removes the monkeys tracks, if the monkey tries to wipe the tarcks from where it started, the monkey dies a gruesome death. Not good :(
	if (attachedTile != nullptr) { attachedTile->hasThief = false; }
	attachedTile = newTile;
	attachedTile->hasThief = true;
	AGameManager::thiefLock = false;
	//steal all the resources
	AGameManager::gameGlobal->stealAll();
	this->SetActorLocation(loc);
}

