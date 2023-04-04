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

void ASettlement::SettlementBuyer(EPlayer player) {
    if (locked || bought) { return; }
    //if (game->CurrentPlayer != player) { return; }
    PlayerInventory* inv = AGameManager::gameGlobal->players->getPlayer(player);
    if (inv->canBuySett()) {
        if (AGameManager::gameGlobal->globalTurn < 9) {
            ABuilding* spawnedBuilding = GetWorld()->SpawnActor<ABuilding>(Level1, GetActorLocation(), FRotator::ZeroRotator);
            building = spawnedBuilding;
            bought = true;
            playerOwner = player;
            ++inv->settlements;
            settlementLock(player);
        }
        else {
            if (this->roadAdjacency(player)) {
                if (inv->buySettlement()) {
                    ABuilding* spawnedBuilding = GetWorld()->SpawnActor<ABuilding>(Level1, GetActorLocation(), FRotator::ZeroRotator);
                    building = spawnedBuilding;
                    bought = true;
                    playerOwner = player;
                    settlementLock(player);
                }
            }
        }
    }
}
//NOT DONE
void ASettlement::Upgrade() {
    if (!upgraded && bought) {
        FVector currentActorLocation = building->GetActorLocation();
        building->Destroy();
        ABuilding* spawnedBuilding = GetWorld()->SpawnActor<ABuilding>(Level2, FVector(currentActorLocation.X, currentActorLocation.Y, 100), FRotator::ZeroRotator);
        building = spawnedBuilding;
        upgraded = true;
    }
}

void ASettlement::AddResource(EResource type) {
    if (playerOwner == EPlayer::NONE) { return; } 
    PlayerInventory* inv = AGameManager::gameGlobal->players->getPlayer(playerOwner);
    inv->addResource(type);
    if (upgraded) { inv->addResource(type); }
}

void ASettlement::stealResource(EPlayer stealer) {
    PlayerInventory* stealInv = AGameManager::gameGlobal->players->getPlayer(stealer);
    PlayerInventory* currInv = AGameManager::gameGlobal->players->getPlayer(playerOwner);
   /////////////////////////////////////////////////////////////////////
}

void ASettlement::settlementLock(EPlayer player) {
    UWorld* world = GetWorld();
    TArray<AActor*> foundActors;
    UGameplayStatics::GetAllActorsOfClass(world, ASettlement::StaticClass(), foundActors);
    for (AActor* Actor : foundActors) {
        if (Actor == this) { continue; }
        if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 750.0f) {
            ASettlement* sett = Cast<ASettlement>(Actor);
            sett->locked = true;
        }
    }
}

bool ASettlement::roadAdjacency(EPlayer player) {
    UWorld* world = GetWorld();
    TArray<AActor*> foundActors;
    UGameplayStatics::GetAllActorsOfClass(world, ARoad::StaticClass(), foundActors);
    for (AActor* Actor : foundActors) {
        if (Actor == this) { continue; }
        if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 750.0f) {
            ARoad* road = Cast<ARoad>(Actor);
            if (road->playerOwner == player) { return true; }
        }
    }
    return false;
}