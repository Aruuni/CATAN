#include "Thief.h"
#include "GameManager.h"
#include "HexTileSpawner.h"
#include <Kismet/GameplayStatics.h>

AThief::AThief() {
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	thiefMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bMesh"));
	thiefMesh->SetupAttachment(RootComponent);

}

void AThief::BeginPlay() {
}
	

void AThief::moveThief(FVector loc, AHexTile* newTile) {
	if (attachedTile != nullptr) {
		attachedTile->hasThief = false;
	}
	attachedTile = newTile;
	attachedTile->hasThief = true;
	AGameManager::thiefLock = true;
	AGameManager::players->stealAll();
	this->SetActorLocation(loc);
}

