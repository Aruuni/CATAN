#include "Road.h"
#include "Settlement.h"
#include "GameManager.h"
#include <Kismet/GameplayStatics.h>

ARoad::ARoad() {}

void ARoad::RoadBuyer(EPlayer player) {
	if (bought) { return; }
	PlayerInventory* inv = AGameManager::gameGlobal->players->getPlayer(player);
	//if (game->CurrentPlayer != player) { return; }
	if (inv->canBuyRoad()) { 
		if (RoadDetector(player) || SettlementDetector(player)) {
			if (AGameManager::gameGlobal->globalTurn < 9) {
				ABuilding* spawnedBuilding = GetWorld()->SpawnActor<ABuilding>(Road, GetActorLocation(), FRotator::ZeroRotator);
				bought = true;
				playerOwner = player;
				++inv->roads;
			}
			else {
				if (inv->buyRoad()) {
					ABuilding* spawnedBuilding = GetWorld()->SpawnActor<ABuilding>(Road, GetActorLocation(), FRotator::ZeroRotator);
					bought = true;
					playerOwner = player;
				}
			}
		}
	}
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
