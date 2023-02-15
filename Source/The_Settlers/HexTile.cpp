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
void AHexTile::thiefMove(EPlayer stealer) {
	for (int8 i = 0; i < 4; ++i) {
		game->playerInventories[i].removeHalf();
	}
	FVector currLoc = thief->GetActorLocation();
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHexTile::StaticClass(), foundActors);
	for (AActor* foundActor : foundActors) {
		AHexTile* hex = Cast<AHexTile>(foundActor);
		hex->hasThief = false;
	}
	TArray<AActor*> foundMONKEY;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AThief::StaticClass(), foundMONKEY);
	for (AActor* Actor : foundMONKEY) {
		AThief* pastthief = Cast<AThief>(Actor);
		if (pastthief) {
			pastthief->Destroy();
		}
	}
	
	// Spawn a new instance of the actor at the location of the clicked tile
	thief = GetWorld()->SpawnActor<AThief>(thiefMesh, GetActorLocation(), FRotator::ZeroRotator);
	this->hasThief = true;
	for (int8 i = 0; i < 6; ++i) {
		if (this->tileType == EHexTile::DESERT) {
			return;
		}
		if (this->settArray[i]->playerOwner == EPlayer::NONE) {
			continue;
		}
		/*if (this->settArray[i]->playerOwner == stealer) {
			continue;
		}*/
		this->settArray[i]->stealResource(stealer);
		return;
	}
}

