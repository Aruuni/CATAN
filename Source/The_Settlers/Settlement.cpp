#include "Settlement.h"
#include "Building.h"
#include "HexTile.h"
#include "Road.h"
#include "GameManager.h"
#include "Engine/World.h"
// this import is necessary for GetAllActorsOfClass() function 
#include <Kismet/GameplayStatics.h>

ASettlement::ASettlement() {}

// the buyer is the function called when an attempt to buy a settlement is made
// player - the player who is attempting to buy the settlement
// returns true if the settlement was bought, false otherwise
bool ASettlement::SettlementBuyer(EPlayer player) {
    //this might be called when the player clicks on the settlement accidentally or erroneously and so this immediately ruminates the function. 
    if (locked || bought) { 
        return false; 
    }
    //makes sure the player trying to buy the settlement is the current player, as a settlement cant be bough out of turn
    if (AGameManager::gameGlobal->CurrentPlayer != player) { 
        return false; 
    }
    //if the turn number is under 9 then all settlements bust be placed for free and with no adjacency requirements
    if (AGameManager::gameGlobal->globalTurn < 9) {
        //buySettlement is called with true to indicate that its free
        if (AGameManager::gameGlobal->getPlayer(player)->buySettlement(true)) {
            //a building is spawned at the location of the settlement with the rotation of a random multiple of 60 
            //the Level1 array of buildings aligns with the EPlayer Enum, so the player is cast to an int and then subtracted by 1 to get the correct index   
            building = GetWorld()->SpawnActor<ABuilding>(Level1[(int32)player - 1], GetActorLocation(), FRotator(0.0f, rand() % 5 * 60, 0.0f));
            bought = true;
            playerOwner = AGameManager::CurrentPlayer;
            //the adjacent settlements are locked
            settlementLock();
            return true;
        }
        return false;
    }
    // after turn 9 the settlement must be placed next to a road and not next to another settlement and not for free hence calling buySettlement with false
    else if (roadAdjacency(player) && AGameManager::gameGlobal->getPlayer(player)->buySettlement(false)){
        //a building is spawned at the location of the settlement with the rotation of a random multiple of 60 
        //the Level1 array of buildings aligns with the EPlayer Enum, so the player is cast to an int and then subtracted by 1 to get the correct index 
        building = GetWorld()->SpawnActor<ABuilding>(Level1[(int32)player - 1], GetActorLocation(), FRotator(0.0f, rand() % 5 * 60, 0.0f));
        bought = true;
        playerOwner = player;
        //the adjacent settlements are locked
        settlementLock();
        return true;
    }
    return false;
}

// used in a blueprint for the UI to check if the settlement has been bought to display the correct error message
// returns true if the settlement has been bought, false otherwise
bool ASettlement::boughtChecker() {
	return bought;
}

// called by the onClick in the blueprint to upgrade the settlement to a city, also played by the bot
// player - the player who is attempting to upgrade the settlement
// returns true if the settlement was upgraded, false otherwise
bool ASettlement::Upgrade(EPlayer player) {
    // a player can only upgrade their own settlement
    if (AGameManager::gameGlobal->CurrentPlayer != player) { 
        return false; 
    }
    // if the settlement has already been upgraded or not bought then the function returns false
    if (!upgraded && bought) {
        if (AGameManager::gameGlobal->getPlayer(AGameManager::CurrentPlayer)->upgradeSettlement() && playerOwner == AGameManager::CurrentPlayer) {
            // the old settlement is destroyed and a new city is spawned in its place
            building->Destroy();
            //similarly to the level1 array, the level2 array aligns with the EPlayer Enum, so the player is cast to an int and then subtracted by 1 to get the correct index
            building = GetWorld()->SpawnActor<ABuilding>(Level2[(int32)AGameManager::CurrentPlayer - 1], FVector(GetActorLocation().X, GetActorLocation().Y, 100), FRotator(0.0f, rand() % 5 * 60, 0.0f));
            upgraded = true;
            return true;
        }
    }
    return false;
}

// this function earns a resource for the player who owns the settlement of the ETileType cast to a EResource
void ASettlement::earnResource(EResource type) {
    // if it has no owner then it returns null 
    if (playerOwner == EPlayer::NONE) { 
        return; 
    } 
    //adds the resource to the player who owns the settlement
    AGameManager::gameGlobal->getPlayer(playerOwner)->addResource(type);
    // it does it again if the settlement has been upgraded
    if (upgraded) { 
        AGameManager::gameGlobal->getPlayer(playerOwner)->addResource(type); 
    }
    // the functions add don't 100% add a resource, it might fail if resources already at max
}

// this function locks all adjacent settlements to the settlement that is being bought to prevent them from being bought 
void ASettlement::settlementLock() {
    TArray<AActor*> foundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
    for (AActor* Actor : foundActors) {
        // prevents this settlement from locking itself
        if (Actor == this) { 
            continue; 
        }
        if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 750.0f) {
            ASettlement* sett = Cast<ASettlement>(Actor);
            //cats the actor into a settlement in order to lock it
            sett->locked = true;
        }
    }
}

// this function checks if the settlement is adjacent to a road of the same player
// player - the player who is attempting to buy the settlement
// returns true if the settlement is adjacent to a road of the same player, false otherwise
bool ASettlement::roadAdjacency(EPlayer player) {
    TArray<AActor*> foundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoad::StaticClass(), foundActors);
    for (AActor* Actor : foundActors) {
        // since roads are quite close to the settlements , the distance is set to 400 to prevent false positives
        if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 400.0f) {
            ARoad* road = Cast<ARoad>(Actor);
            if (road->playerOwner == player) { 
                return true; 
            }
        }
    }
    return false;
}

// function is used in the blueprint for validation of the correct error message
// returns the player who owns the settlement represented by their enum
EPlayer ASettlement::getOwner() {
    return playerOwner;
}

// this function is used to click on a settlement and steal a random resource from the player who owns it
bool ASettlement::stealResource(EPlayer stealer) {
    // checks if the steal lock is active AND the settlement is adjacent to the robber AND the settlement is owned by a player
    // all three of these conditions must be true for the steal function to work
    if (AGameManager::stealLock && thiefAdjacency() && (playerOwner != stealer && playerOwner != EPlayer::NONE)) {
        // a resource to add is randomly selected from the player who owns the settlement
        // it calls the removeOneRand function which returns a random resource and removes it from the player through the global pointer to the game manager
        EResource toAdd = AGameManager::gameGlobal->getPlayer(playerOwner)->removeOneRand();
        // if the resource is none then the function returns false, as the player has no resources to steal, allowing the stealer to try again
        if (toAdd == EResource::NONE) {
            return false;
        }
        // if the resource is added successfully then the steal lock is deactivated and the function returns true
        if (AGameManager::gameGlobal->getPlayer(stealer)->addResource(toAdd)) {
            // releases the 
            AGameManager::stealLock = false;
            return true;
        }
        //if the resource is not added successfully then the resource is added back to the player who owned the settlement to allow the stealer to try again
        AGameManager::gameGlobal->getPlayer(playerOwner)->addResource(toAdd);
        return false;
    }
    return false;
}

// this function checks if the settlement is adjacent to the thief
// returns true if the settlement is adjacent to the thief, false otherwise
bool ASettlement::thiefAdjacency() {
    TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHexTile::StaticClass(), foundActors);
    for (AActor* Actor : foundActors) {
        if (FVector::Dist(Actor->GetActorLocation(), GetActorLocation()) <= 600.0f) {
            AHexTile* tile = Cast<AHexTile>(Actor);
            // if the tile has the thief then the function returns true and this means the settlement is close enough to the thief to allow the steal function to work
			if (tile->hasThief) { 
                return true; 
            }
		}
	}
	return false;
}