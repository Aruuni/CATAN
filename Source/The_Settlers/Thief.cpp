#include "Thief.h"
#include <Kismet/GameplayStatics.h>

// the constructor of the thief, it is responsible for setting the root component and the mesh of the thief, also assigns the global pointer to the thief
AThief::AThief() {
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	thiefMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bMesh"));
	thiefMesh->SetupAttachment(RootComponent);
	thief = this;
}

// function responsible for moving the thief, it is called from the tile blueprint , when the player clicks on a tile or automatically by a bot
// newTile - the pointer to the tile that the player clicked on/ bot wishes to move the thief to
void AThief::moveThief(AHexTile* newTile) {
	if (!AGameManager::thiefLock) { return; }
	// if the tile is not a resource tile, the thief cannot move there, it was able to move there before and it was kinda funny, but it was not intended.
	if ((int32)newTile->tileType > 5) { return; }
	//Removes the monkeys tracks, if the monkey tries to wipe the tracks from where it started, the monkey dies a gruesome death. Not good :(
	if (attachedTile != nullptr) { attachedTile->hasThief = false; }
	attachedTile = newTile;
	attachedTile->hasThief = true;
	AGameManager::thiefLock = false;
	// turns on the steal lock, so that the player can steal a resource from another player's settlement
	AGameManager::stealLock = true;
	// moves the thief to the new tile in the world
	this->SetActorLocation(newTile->GetActorLocation());
}

