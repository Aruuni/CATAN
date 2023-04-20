#include "HexTile.h"
#include <Components/SceneCaptureComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>

class AHexSpawner;

AHexTile::AHexTile() {
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(RootComponent);
}
void AHexTile::settSet() {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
	for (AActor* Actor : foundActors) {
		if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 1000.0f) {
			settArray.Add(Cast<ASettlement>(Actor));
		}
	}
}



//only used for trade tiles 
bool AHexTile::checkMySettlementAdjacency(EPlayer player) {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
	for (AActor* Actor : foundActors) {
		if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 1000.0f) {
			if (Cast<ASettlement>(Actor)->playerOwner == player) {
				return true;
			}
		}
	}
	return false;
}

void AHexTile::earnResource() {
	for (int8 sett = 0; sett < 6; ++sett) {
		settArray[sett]->earnResource((EResource)tileType);
	}
}

