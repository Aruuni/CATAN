#include "Road.h"
#include "Settlement.h"
#include "GameManager.h"
#include <Kismet/GameplayStatics.h>

ARoad::ARoad() {}
void ARoad::RoadBuyer(EPlayer player) {
	if (!bought) {
		FPlayerInventory inv;
		UWorld* world = GetWorld();
		TArray<AActor*> foundActors;
		UGameplayStatics::GetAllActorsOfClass(world, AGameManager::StaticClass(), foundActors);
		for (AActor* foundActor : foundActors) {
			AGameManager* game = Cast<AGameManager>(foundActor);
			inv = game->playerInventories[(int32)player];
			//if (game->CurrentPlayer == owner) {
			if (inv.canBuyRoad) {
				if (inv.roads < 15) {
					if (this->RoadDetector(player) || this->SettlementDetector(player)) {
						if (game->globalTurn < 9) {
							ABuilding* spawnedBuilding = GetWorld()->SpawnActor<ABuilding>(Road, GetActorLocation(), FRotator::ZeroRotator);
							this->bought = true;
							this->playerOwner = player;
						}
						else {
							if (inv.bricks >= 1 && inv.wood >= 1 && inv.wool >= 1 && inv.wheat >= 1) {
								ABuilding* spawnedBuilding = GetWorld()->SpawnActor<ABuilding>(Road, GetActorLocation(), FRotator::ZeroRotator);
								this->bought = true;
								this->playerOwner = player;
								--inv.bricks;
								--inv.wood;
								--inv.wool;
								--inv.wheat;
							}
						}
					}
				}
				//inv.canBuyRoad = false;
			}
			//}
		}
		for (AActor* foundActor : foundActors) {
			AGameManager* game = Cast<AGameManager>(foundActor);
			game->playerInventories[(int32)player] = inv;
		}
	}
}

bool ARoad::RoadDetector(EPlayer player) {
	UWorld* world = GetWorld();
	TArray<AActor*> foundActors;

	UGameplayStatics::GetAllActorsOfClass(world, ARoad::StaticClass(), foundActors);
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
	UWorld* world = GetWorld();
	TArray<AActor*> foundActors;

	UGameplayStatics::GetAllActorsOfClass(world, ASettlement::StaticClass(), foundActors);
	for (AActor* Actor : foundActors) {
		if (Actor == this) { continue; }
		if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 470.0f) {
			ASettlement* sett = Cast<ASettlement>(Actor);
			if (sett->playerOwner == player) { return true; }
		}
	}
	return false;
}

