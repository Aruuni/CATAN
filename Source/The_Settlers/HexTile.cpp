#include "HexTile.h"
#include "Thief.h"
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
			ASettlement* Sett = Cast<ASettlement>(Actor);
			settArray.Add(Sett);
		}
	}
}



//only used for trade tiles 
bool AHexTile::checkMySettlementAdjacency(EPlayer player) {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
	for (AActor* Actor : foundActors) {
		if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 1000.0f) {
			ASettlement* Sett = Cast<ASettlement>(Actor);
			if (Sett->playerOwner == player) {
				return true;
			}
		}
	}
	return false;
}

