#include "Settlement.h"
#include "Building.h"
#include "Road.h"
#include "GameManager.h"
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>

ASettlement::ASettlement() {}

void  ASettlement::BeginPlay() {
    Super::BeginPlay();
}

bool ASettlement::SettlementBuyer(EPlayer player) {
    if (locked || bought) { return false; }
    //if (AGameManager::gameGlobal->CurrentPlayer != player) { return false; }
    if (AGameManager::gameGlobal->getPlayer(player)->buySettlement(AGameManager::gameGlobal->globalTurn < 9) || (roadAdjacency(player) && AGameManager::gameGlobal->getPlayer(player)->buySettlement(AGameManager::gameGlobal->globalTurn < 9))) {
        building = GetWorld()->SpawnActor<ABuilding>(Level1[(int32)player - 1], GetActorLocation(), FRotator(0.0f, rand() % 5 * 60, 0.0f));
        bought = true;
        playerOwner = player;
        settlementLock(player);
        return true;
    }
        
    
    return false;
}
bool ASettlement::boughtChecker() {
	return bought;
}
//NOT DONE needs validation
bool ASettlement::Upgrade(EPlayer player) {
    //if (AGameManager::gameGlobal->CurrentPlayer != player) { return false; }
    if (!upgraded && bought) {
        if (AGameManager::gameGlobal->getPlayer(player)->upgradeSettlement()) {
            FVector currentActorLocation = building->GetActorLocation();
            building->Destroy();
            building = GetWorld()->SpawnActor<ABuilding>(Level2[(int32)player - 1], FVector(currentActorLocation.X, currentActorLocation.Y, 100), FRotator(0.0f, rand() % 5 * 60, 0.0f));
            upgraded = true;
            return true;
        }
    }
    return false;
}

void ASettlement::AddResource(EResource type) {
    if (playerOwner == EPlayer::NONE) { return; } 
    AGameManager::gameGlobal->getPlayer(playerOwner)->addResource(type);
    if (upgraded) { AGameManager::gameGlobal->getPlayer(playerOwner)->addResource(type); }
}

void ASettlement::stealResource(EPlayer stealer) {
    PlayerInventory* stealInv = AGameManager::gameGlobal->getPlayer(stealer);
    PlayerInventory* currInv = AGameManager::gameGlobal->getPlayer(playerOwner);
   /////////////////////////////////////////////////////////////////////
}

void ASettlement::settlementLock(EPlayer player) {
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
        if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 750.0f) {
            ARoad* road = Cast<ARoad>(Actor);
            if (road->playerOwner == player) { return true; }
        }
    }
    return false;
}