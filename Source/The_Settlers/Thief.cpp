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
	//Removes the monkeys tracks, if the monkey tries to wipe the tarcks from where it started, the monkey dies a gruesome death. Not good :(
	if (attachedTile != nullptr) { attachedTile->hasThief = false; }
	attachedTile = newTile;
	attachedTile->hasThief = true;
	AGameManager::thiefLock = false;
	AGameManager::gameGlobal->stealAll();
	this->SetActorLocation(loc);
}

