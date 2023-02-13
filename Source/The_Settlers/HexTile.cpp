#include "HexTile.h"
#include "Thief.h"
#include "HexTileSpawner.h"
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
void AHexTile::thiefMove() {
	hexSpawner->resetThief();
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AThief::StaticClass(), foundActors);
	for (AActor* Actor : foundActors) {
		AThief* pastthief = Cast<AThief>(Actor);
		pastthief->Destroy();
	}
	// Spawn a new instance of the actor at the location of the clicked tile
	GetWorld()->SpawnActor<AThief>(thiefMesh, GetActorLocation(), FRotator::ZeroRotator);
	this->hasThief = true;
}

