#include "Road.h"
#include "Settlement.h"
#include "GameManager.h"
#include <Kismet/GameplayStatics.h>

ARoad::ARoad() {}

bool ARoad::RoadBuyer(EPlayer player) {
	if (bought) { return false; }
	if (AGameManager::gameGlobal->CurrentPlayer != player) { return false; }
	if (AGameManager::gameGlobal->globalTurn < 9) {
		if (settlementAdjacency(player) && AGameManager::gameGlobal->getPlayer(player)->buyRoad(true)) {
			GetWorld()->SpawnActor<ABuilding>(roads[(int32)player - 1], GetActorLocation(), GetActorRotation());
			bought = true;
			playerOwner = player;
			return true;
		}
		return false;
	}
	else if (roadAdjacency(player) || settlementAdjacency(player) ) {
		if (AGameManager::gameGlobal->getPlayer(player)->buyRoad(false)) {
			GetWorld()->SpawnActor<ABuilding>(roads[(int32)player - 1], GetActorLocation(), GetActorRotation());
			bought = true;
			playerOwner = player;
			return true;
		}
	}
	return false;
}

bool ARoad::roadAdjacency(EPlayer player) {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoad::StaticClass(), foundActors);
	for (AActor* Actor : foundActors) {
		if (Actor == this) { continue; }
		if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 470.0f) {
			ARoad* road = Cast<ARoad>(Actor);
			if (road->playerOwner == player) { return true; }
		}
	}
	return false;
}

bool ARoad::settlementAdjacency(EPlayer player) {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
	for (AActor* Actor : foundActors) {
		if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 470.0f) {
			ASettlement* sett = Cast<ASettlement>(Actor);
			if (sett->playerOwner == player) { return true; }
		}
	}
	return false;
}

// make it happen 
// for all settlement adjacenty that isnt mine
