#include "Settlement.h"
#include "Building.h"
#include "Road.h"
#include "GameManager.h"
#include "Engine/World.h"
#include <Kismet/GameplayStatics.h>
ASettlement::ASettlement() {}
void ASettlement::SettlementBuyer(EPlayer player) {
    if (!locked) {
        if (!bought) {
            FPlayerInventory inv = game->playerInventories[(int32)player];
            //but mihai, why do you have 4 nested ifs i hear you asking, well, suck my nuts, its either ugly code or ugly blueprint
            //if (game->CurrentPlayer == owner) {
            if (inv.settlements < 6) {
                if (inv.canBuySett) {
                    if (game->globalTurn < 9) {
                        ABuilding* spawnedBuilding = GetWorld()->SpawnActor<ABuilding>(Level1, GetActorLocation(), FRotator::ZeroRotator);
                        building = spawnedBuilding;
                        this->bought = true;
                        this->playerOwner = player;
                        ++inv.settlements;
                        this->settlementLock(player);
                    }
                    else {
                        if (this->roadAdjacency(player)) {
                            if (inv.bricks >= 1 && inv.wood >= 1 && inv.wool >= 1 && inv.wheat >= 1) {
                                ABuilding* spawnedBuilding = GetWorld()->SpawnActor<ABuilding>(Level1, GetActorLocation(), FRotator::ZeroRotator);
                                building = spawnedBuilding;
                                this->bought = true;
                                this->playerOwner = player;
                                --inv.bricks;
                                --inv.wood;
                                --inv.wool;
                                --inv.wheat;
                                ++inv.settlements;
                                this->settlementLock(player);
                            }
                        }
                    }
                    //inv.canBuySett = false;
                    game->playerInventories[(int32)player] = inv;
                }
            }
            //}
        }
    }
}
//not done
void ASettlement::Upgrade() {
    if (!upgraded && bought) {
        FVector currentActorLocation = building->GetActorLocation();
        building->Destroy();
        ABuilding* spawnedBuilding = GetWorld()->SpawnActor<ABuilding>(Level2, FVector(currentActorLocation.X, currentActorLocation.Y, 100), FRotator::ZeroRotator);
        building = spawnedBuilding;
        this->upgraded = true;
    }
}

void ASettlement::AddResource(EResource type) {
    if (playerOwner != EPlayer::NONE) {
        FPlayerInventory inv;
        inv = game->playerInventories[(int32)this->playerOwner - 1];
        // this bit here needs rethinking, you can get more then 19 resources if the settlementis 
        if (checkMaxResources(type) == 19) {
            return;
        }
        if (type == EResource::BRICKS) { inv.bricks = !upgraded ? ++inv.bricks : inv.bricks += 2; }
        if (type == EResource::ORE) { inv.ore = !upgraded ? ++inv.ore : inv.ore += 2; }
        if (type == EResource::WHEAT) { inv.wheat = !upgraded ? ++inv.wheat : inv.wheat += 2; }
        if (type == EResource::WOOL) { inv.wool = !upgraded ? ++inv.wool : inv.wool += 2; }
        if (type == EResource::WOOD) { inv.wood = !upgraded ? ++inv.wood : inv.wood += 2; }
        game->playerInventories[(int32)this->playerOwner - 1] = inv;
    }
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

void  ASettlement::BeginPlay() {
    Super::BeginPlay();
    TArray<AActor*> foundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameManager::StaticClass(), foundActors);
    for (AActor* foundActor : foundActors) {
        game = Cast<AGameManager>(foundActor);
    }
}

int32 ASettlement::checkMaxResources(EResource resource) {
    if (EResource::WHEAT == resource) {
        int32 totwheat = 0;
        for (int8 i = 0; i < 4; ++i) {
            totwheat += game->playerInventories[i].wheat;
        }
        return totwheat;
    }
    if (EResource::WOOL == resource) {
        int32 totwool = 0;
        for (int8 i = 0; i < 4; ++i) {
            totwool += game->playerInventories[i].wool;
        }
        return totwool;
    }
    if (EResource::WOOD == resource) {
        int32 totwood = 0;
        for (int8 i = 0; i < 4; ++i) {
            totwood += game->playerInventories[i].wood;
        }
        return totwood;

    }
    if (EResource::ORE == resource) {
        int32 totore = 0;
        for (int8 i = 0; i < 4; ++i) {
            totore += game->playerInventories[i].ore;
        }
        return totore;
    }
    if (EResource::BRICKS == resource) {
        int32 totbricks = 0;
        for (int8 i = 0; i < 4; ++i) {
            totbricks += game->playerInventories[i].bricks;
        }
        return totbricks;
    }
    return 0;
}