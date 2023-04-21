#include "Settlement.h"
#include "Building.h"
#include "HexTile.h"
#include "Road.h"
#include "GameManager.h"
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>

ASettlement::ASettlement() {}


bool ASettlement::SettlementBuyer(EPlayer player) {
    if (locked || bought) { return false; }
    if (AGameManager::gameGlobal->CurrentPlayer != player) { return false; }
    if (AGameManager::gameGlobal->globalTurn < 9) {
        if (AGameManager::gameGlobal->getPlayer(player)->buySettlement(true)) {
            building = GetWorld()->SpawnActor<ABuilding>(Level1[(int32)player - 1], GetActorLocation(), FRotator(0.0f, rand() % 5 * 60, 0.0f));
            bought = true;
            playerOwner = AGameManager::CurrentPlayer;
            settlementLock();
            return true;
        }
        return false;
    }
    else if (roadAdjacency(player) && AGameManager::gameGlobal->getPlayer(player)->buySettlement(false)){
        building = GetWorld()->SpawnActor<ABuilding>(Level1[(int32)player - 1], GetActorLocation(), FRotator(0.0f, rand() % 5 * 60, 0.0f));
        bought = true;
        playerOwner = player;
        settlementLock();
        return true;
    }
    return false;
}

bool ASettlement::boughtChecker() {
	return bought;
}

bool ASettlement::Upgrade(EPlayer player) {
    if (AGameManager::gameGlobal->CurrentPlayer != player) { return false; }
    if (!upgraded && bought) {
        if (AGameManager::gameGlobal->getPlayer(AGameManager::CurrentPlayer)->upgradeSettlement() && playerOwner == AGameManager::CurrentPlayer) {
            FVector currentActorLocation = building->GetActorLocation();
            building->Destroy();
            building = GetWorld()->SpawnActor<ABuilding>(Level2[(int32)AGameManager::CurrentPlayer - 1], FVector(currentActorLocation.X, currentActorLocation.Y, 100), FRotator(0.0f, rand() % 5 * 60, 0.0f));
            upgraded = true;
            return true;
        }
    }
    return false;
}

void ASettlement::earnResource(EResource type) {
    if (playerOwner == EPlayer::NONE) { return; } 
    AGameManager::gameGlobal->getPlayer(playerOwner)->addResource(type);
    if (upgraded) { AGameManager::gameGlobal->getPlayer(playerOwner)->addResource(type); }
}

bool ASettlement::stealResource(EPlayer stealer) {
    if (!AGameManager::stealLock) { return false; }
    if (thiefAdjacency() && (playerOwner != stealer && playerOwner != EPlayer::NONE)) {
        EResource toAdd = AGameManager::gameGlobal->getPlayer(playerOwner)->removeOneRand();
        if (toAdd == EResource::NONE) { return false; }
        if (AGameManager::gameGlobal->getPlayer(stealer)->addResource(toAdd)) {
            AGameManager::stealLock = false;
            return true;
        }
        AGameManager::stealLock = false;
        return true;
    }
    return false;
}

void ASettlement::settlementLock() {
    TArray<AActor*> foundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
    for (AActor* Actor : foundActors) {
        if (Actor == this) { continue; }
        if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 750.0f) {
            ASettlement* sett = Cast<ASettlement>(Actor);
            sett->locked = true;
        }
    }
}

bool ASettlement::roadAdjacency(EPlayer player) {
    TArray<AActor*> foundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoad::StaticClass(), foundActors);
    for (AActor* Actor : foundActors) {
        if (Actor == this) { continue; }
        if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 400.0f) {
            ARoad* road = Cast<ARoad>(Actor);
            if (road->playerOwner == player) { return true; }
        }
    }
    return false;
}

EPlayer ASettlement::getOwner() {
    return playerOwner;
}

bool ASettlement::thiefAdjacency() {
    TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHexTile::StaticClass(), foundActors);
    for (AActor* Actor : foundActors) {
		if (Actor == this) { continue; }
        if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 600.0f) {
            AHexTile* tile = Cast<AHexTile>(Actor);
			if (tile->hasThief) { return true; }
		}
	}
	return false;
}