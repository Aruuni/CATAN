#include "Road.h"
#include "Settlement.h"
#include "GameManager.h"
#include <Kismet/GameplayStatics.h>

ARoad::ARoad() {}

bool ARoad::RoadBuyer(EPlayer player) {
	if (bought) { return false; }
	//if (game->CurrentPlayer != player) { return false; }
	if (AGameManager::gameGlobal->getPlayer(player)->canBuyRoad()) {
		if (RoadDetector(player) || SettlementDetector(player)) {
			if (AGameManager::gameGlobal->globalTurn < 9) {
				ABuilding* spawnedBuilding = GetWorld()->SpawnActor<ABuilding>(roads[(int32)AGameManager::gameGlobal->getCurrentPlayer()-1], GetActorLocation(), GetActorRotation());
				bought = true;
				playerOwner = player;
				++AGameManager::gameGlobal->getPlayer(player)->roads;
				return true;
			}
			else {
				if (AGameManager::gameGlobal->getPlayer(player)->buyRoad()) {
					ABuilding* spawnedBuilding = GetWorld()->SpawnActor<ABuilding>(roads[(int32)AGameManager::gameGlobal->getCurrentPlayer() - 1], GetActorLocation(), GetActorRotation());
					bought = true;
					playerOwner = player;
					return true;
				}
			}
		}
	}
	return false;
}

bool ARoad::RoadDetector(EPlayer player) {
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

bool ARoad::SettlementDetector(EPlayer player) {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
	for (AActor* Actor : foundActors) {
		if (Actor == this) { continue; }
		if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 470.0f) {
			ASettlement* sett = Cast<ASettlement>(Actor);
			if (sett->playerOwner == player) { return true; }
		}
	}
	return false;
}

