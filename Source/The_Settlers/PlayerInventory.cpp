#include "PlayerInventory.h"
#include "ENUMS.h"	
#include "GameManager.h"

PlayerInventory::PlayerInventory(EPlayer player) {
	this->player = player;
	this->Resources = {2,2,2,2,2};
}


#pragma region BORING GETTERS

bool PlayerInventory::canBuyRoad() { return canBuyRoadbool && roads <=15; }
bool PlayerInventory::canBuySett() { return canBuySettbool && settlements<=5; }
bool PlayerInventory::canUpgrade() { return canUpgradebool && cities <=5; }

#pragma endregion

#pragma region BuyMethods

bool PlayerInventory::buyRoad(bool free) {
	if (!canBuyRoad()) { return false; }
	if (free) {
		++roads;
		canBuyRoadbool = false;
		return true;
	}
	else if (Resources[(int32)EResource::BRICKS] >= 1 && Resources[(int32)EResource::WOOD] >= 1) {
		--Resources[(int32)EResource::WOOD]; --AGameManager::gResources[(int32)EResource::WOOD];
		--Resources[(int32)EResource::BRICKS]; --AGameManager::gResources[(int32)EResource::BRICKS];
		++roads;
		canBuyRoadbool = false;
		return true;
	}
	return false; 
}

bool PlayerInventory::buySettlement(bool free) {
	if (!canBuySett()) { return false; }
	if (free) {
		++settlements;
		++victoryPoints;
		canBuySettbool = false;
		return true;
	}
	else if (Resources[(int32)EResource::WHEAT] >= 1 && Resources[(int32)EResource::BRICKS] >= 1 && Resources[(int32)EResource::WOOL] >= 1 && Resources[(int32)EResource::WOOD] >= 1) {
		--Resources[(int32)EResource::WHEAT]; --AGameManager::gResources[(int32)EResource::WHEAT];
		--Resources[(int32)EResource::WOOD];  --AGameManager::gResources[(int32)EResource::WOOD];
		--Resources[(int32)EResource::WOOL];  --AGameManager::gResources[(int32)EResource::WOOL];
		--Resources[(int32)EResource::BRICKS]; --AGameManager::gResources[(int32)EResource::BRICKS];
		++settlements;
		++victoryPoints;
		canBuySettbool = false;
		return true;
	}
	else { return false; }
}

bool PlayerInventory::upgradeSettlement() {
	if (canUpgrade() && (Resources[(int32)EResource::WHEAT] >= 2 && Resources[(int32)EResource::ORE] >= 3)) {
		----Resources[(int32)EResource::WHEAT]; ----AGameManager::gResources[(int32)EResource::WHEAT];
		------Resources[(int32)EResource::ORE]; ------AGameManager::gResources[(int32)EResource::ORE];
		--settlements;
		++cities;
		++victoryPoints;
		canUpgradebool = false;
		return true;
	}
	return false;
}

#pragma endregion

#pragma region Cards

int32 PlayerInventory::getKnights() {
	return knights;
}

int32 PlayerInventory::getCardCount(ECards cardC) {
	int32 count = 0;
	for (ECards card : hand) {
		if (card == cardC) {
			++count;
		}
	}
	return count;
}

int32 PlayerInventory::getVP() {
	return victoryPoints;
}

bool PlayerInventory::drawCard() {
	if (canDrawCardbool && (Resources[(int32)EResource::ORE] >= 1 && Resources[(int32)EResource::WHEAT] >= 1 && Resources[(int32)EResource::WOOL] >= 1)) {
		--Resources[(int32)EResource::ORE]; --AGameManager::gResources[(int32)EResource::ORE];
		--Resources[(int32)EResource::WHEAT]; --AGameManager::gResources[(int32)EResource::WHEAT];
		--Resources[(int32)EResource::WOOL]; --AGameManager::gResources[(int32)EResource::WOOL];
		int32 randno = rand() % AGameManager::Gdeck.Num();
		hand.Add((AGameManager::Gdeck)[randno]);
		unplayable = (AGameManager::Gdeck)[randno];
		AGameManager::Gdeck.RemoveAt(randno);
		canDrawCardbool = false;
		return true;
	}
	 return false; 
}

bool PlayerInventory::useCard(ECards card) {
	if (card == unplayable) { if (!moreThanOne(card)) { return false; } }
	int32 cardIndex = hand.Find(card);
	if (cardIndex != INDEX_NONE) {
		ECards removedCard = this->hand[cardIndex];
		hand.RemoveAt(cardIndex);
		return true;
	}
	return false;
}

bool PlayerInventory::moreThanOne(ECards card){
	int count = 0;
	for (ECards c : hand) { if (c == card) { count; } }
	return count>=2 ? true:false;
}

void PlayerInventory::addKnight() {
	++knights;
}

void PlayerInventory::addVictoryPoint() {
	++victoryPoints;
}


#pragma endregion

#pragma region Inventory Management

void PlayerInventory::reset() {
	canBuyRoadbool = true;
	canBuySettbool = true;
	canUpgradebool = true;
	canDrawCardbool = true;
	canPlayCardbool = true;
	unplayable = ECards::NONE;
}

void PlayerInventory::removeHalf() {
	int16 totalRes = total();
	if (totalRes < 8) { return; }
	for (int8 i = 0; i < totalRes / 2; ++i) { removeOneRand(); }
}

int32 PlayerInventory::total() { return Resources[0] + Resources[1] + Resources[2] + Resources[3] + Resources[4]; }

void PlayerInventory::removeOneRand(){
rereoll:
	if (Resources[(int32)EResource::WHEAT] == 0 && Resources[(int32)EResource::BRICKS] == 0 && Resources[(int32)EResource::ORE] == 0 && Resources[(int32)EResource::WOOD] == 0 && Resources[(int32)EResource::WOOL] == 0) { return; }
	int randomResource = rand() % 5;
	if (Resources[randomResource] == 0) { goto rereoll; }
	--Resources[randomResource]; 
	--AGameManager::gResources[randomResource];
}

bool PlayerInventory::addResource(EResource resource) {
	if (AGameManager::gResources[(int32)resource] <= 18) { ++Resources[(int32)resource]; ++AGameManager::gResources[(int32)resource]; return true; }
	return false;
}

bool PlayerInventory::removeResource(EResource resource) {
	if (Resources[(int32)resource] > 0) { --Resources[(int32)resource]; --AGameManager::gResources[(int32)resource]; return true; }
	return false;
}


int32 PlayerInventory::getResource(EResource resource) {
	return Resources[(int32)resource];
}

#pragma endregion


 