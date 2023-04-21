#include "Road.h"
#include "Settlement.h"
#include "GameManager.h"
#include <Kismet/GameplayStatics.h>

ARoad::ARoad() {}

// the buyer is the function called when an attempt to buy a road is made
// player - the player who is attempting to buy the road
// returns true if the road was bought, false otherwise
bool ARoad::RoadBuyer(EPlayer player) {
	if (bought) { return false; }
	if (AGameManager::gameGlobal->CurrentPlayer != player) { return false; }
	if (AGameManager::gameGlobal->globalTurn < 9) {
		// buy road is called with true to indicate that the road is free
		if (roadAdjacency(player)) {
			return false;
		}
		if (settlementAdjacency(player) && AGameManager::gameGlobal->getPlayer(player)->buyRoad(true)) {
			// a building of the corresponding color is spawned at the location of the settlement with the rotation of the road base, this is important as otherwise the road will be incorrectly and look strange
			GetWorld()->SpawnActor<ABuilding>(roads[(int32)player - 1], GetActorLocation(), GetActorRotation());
			bought = true;
			playerOwner = player;
			return true;
		}
		return false;
	}
	// if there is road adjacency and no enemy settlement adjacency then the road placement is valid
	else if (roadAdjacency(player) && !enemySettlementAdjacency(player)) {
		// buy road is called with false to indicate that the road is not free
		if (AGameManager::gameGlobal->getPlayer(player)->buyRoad(false)) {
			// a building of the corresponding color is spawned at the location of the settlement with the rotation of the road base, this is important as otherwise the road will be incorrectly and look strange
			GetWorld()->SpawnActor<ABuilding>(roads[(int32)player - 1], GetActorLocation(), GetActorRotation());
			bought = true;
			playerOwner = player;
			return true;
		}
	}
	return false;
}

// checks if there is a road of the same player within 470 units of the road owned by the player
// player - the player who is attempting to buy the road
// returns true if there is a road of the same player within 470 units of the road owned by the player, false otherwise
bool ARoad::roadAdjacency(EPlayer player) {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoad::StaticClass(), foundActors);
	for (AActor* Actor : foundActors) {
		// ignores this road to prevent it from false positives when finding a road in rage 
		if (Actor == this) { 
			continue; 
		}
		// uses the built in distance function to check if the road is within 470 units of the settlement
		if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 470.0f) {
			ARoad* road = Cast<ARoad>(Actor);
			if (road->playerOwner == player) { 
				return true; 
			}
		}
	}
	return false;
}
// checks if there is a settlement of the same player within 470 units of the road owned by the player, this is only used in the first 8 turns as if a settlement is close enough to the road to be placed the it must mean that there is also a road close enough to the settlement to be placed
// player - the player who is attempting to buy the road
// returns true if there is a settlement of the same player within 470 units of the road owned by the player, false otherwise
bool ARoad::settlementAdjacency(EPlayer player) {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
	for (AActor* Actor : foundActors) {
		if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 470.0f) {
			ASettlement* sett = Cast<ASettlement>(Actor);
			if (sett->playerOwner == player) { 
				return true; 
			}
		}
	}
	return false;
}

// checks if there is an enemy settlement within 470 units of the road owned by the player, this is done to prevent roads from being placed adjacent to enemy settlements
// player - the player who is attempting to buy the road
// returns true if there is an enemy settlement within 470 units of the road owned by the player, false otherwise
bool ARoad::enemySettlementAdjacency(EPlayer player) {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
	for (AActor* Actor : foundActors) {
		if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 470.0f) {
			ASettlement* sett = Cast<ASettlement>(Actor);
			// none has to be ignored as it is not an enemy settlement but it will trigger a false positive if not ignored
			if (sett->playerOwner == EPlayer::NONE) {
				continue; 
			}
			if (sett->playerOwner != player) {
				return true; 
			}
		}
	}
	return false;
}

