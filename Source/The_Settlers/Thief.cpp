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
	

void AThief::moveThief(FVector loc, AHexTile* newLocation) {
	attchedTile = newLocation;
	AGameManager::thiefLock = true;
	this->SetActorLocation(loc);
}

