#include "HexTile.h"
#include <Components/SceneCaptureComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>

class AHexSpawner;

// constructor responsible for the mesh and attaching it to the root component
AHexTile::AHexTile() {
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(RootComponent);
}

// called when the hexTile is spawned by the hexSpawner
void AHexTile::settSet() {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
	//loops through all settlements and checks if they are within 1000 units of the hexTile
	for (AActor* Actor : foundActors) {
		if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 1000.0f) {
			// adds the settlement to the array, but first casts it to a settlement as the GetAllActorsOfClass returns an array of AActors
			settArray.Add(Cast<ASettlement>(Actor));
		}
	}
}

// only used for trade tiles to validate that a settlement of the plater attempting to trade is adjacent, works on the same principle as settSet
bool AHexTile::checkMySettlementAdjacency(EPlayer player) {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
	for (AActor* Actor : foundActors) {
		if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 1000.0f) {
			//casts the actor to a settlement and checks if the player of the settlement is the same as the player attempting to trade
			if (Cast<ASettlement>(Actor)->playerOwner == player) {
				return true;
			}
		}
	}
	return false;
}

// the earn resource function loops through all settlements and calls the earn resource function of the settlement
void AHexTile::earnResource() {
	for (int8 sett = 0; sett < 6; ++sett) {
		settArray[sett]->earnResource((EResource)tileType);
	}
}

