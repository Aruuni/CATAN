#include "PlayerInventory.h"
#include "ENUMS.h"	
#include "GameManager.h"

// constructor for the player inventory
PlayerInventory::PlayerInventory(EPlayer player, int32 res) {
	this->player = player;
	this->Resources = { res,res,res,res,res };
}

// getters for if a player is allowed to buy a building
#pragma region Getters 

// responsible for checking if a player can buy a road
// returns true if a player is allowed to buy a road this turn, false otherwise
bool PlayerInventory::canBuyRoad() { 
	// returns true if the player can buy a road this turn and they have not reached the cap of 15 roads or if they can or have used the road building card and have not reached the cap of 15 roads and built the  two roads allowed by the card
	return ( canBuyRoadbool && roads <15 ) || (AGameManager::roadBuildingLock && AGameManager::freeRoadsCount < 2 && roads < 15 )  ; 
}

// responsible for checking if a player can buy a settlement
// returns true if a player is allowed to buy a settlement this turn, false otherwise
bool PlayerInventory::canBuySett() { 
	// checks the cap as well as the lock
	return canBuySettbool && settlements < 5; 
}

// responsible for checking if a player can upgrade a settlement to a city
// returns true if a player is allowed to upgrade a settlement to a city this turn, false otherwise
bool PlayerInventory::canUpgrade() { 
	// checks the cap as well as the lock
	return canUpgradebool && cities <5; 
}


#pragma endregion

// methods responsible for buying roads, settlements and cities
#pragma region BuyMethods

// methods responsible for buying roads, it is responsible for checking if the player has the resources to buy the road and if they have already bought one this turn
// free - if called with the free road card or within the first eight turns of the game
// returns true if the road was bought successfully, false otherwise
bool PlayerInventory::buyRoad(bool free) {
	// if a player cant buy a road this turn or they reached the cap, return false
	if (canBuyRoad()) { 
		// if called with free, it will not check for resources and will just add a road
		if (free) {
			// a road is bought for free
			++roads;
			canBuyRoadbool = false;
			return true;
		}
		// this is the case in which a player has the road building card and has not built two roads yet
		else if (AGameManager::roadBuildingLock && AGameManager::freeRoadsCount < 2) {
			++roads;
			// the global free roads count is incremented so that the next player can use the free road card, this does not lock the player from buying a road
			++AGameManager::freeRoadsCount;
			return true;
		}
		// checks if the player has the resources to buy a road
		else if (Resources[(int32)EResource::BRICKS] >= 1 && Resources[(int32)EResource::WOOD] >= 1) {
			// removes the resources from the player's hand
			--Resources[(int32)EResource::WOOD]; 
			--AGameManager::gResources[(int32)EResource::WOOD];

			--Resources[(int32)EResource::BRICKS]; 
			--AGameManager::gResources[(int32)EResource::BRICKS];
			++roads;
			// locks a road from being bought this turn
			canBuyRoadbool = false;
			return true;
		}
	}
	return false; 
}

// methods responsible for buying settlements,  it is responsible for checking if the player has the resources to buy the settlements and if they have already bought one this turn
// free - if called with the free boolean within the first eight turns of the game
// returns true if the settlements was bought successfully, false otherwise
bool PlayerInventory::buySettlement(bool free) {
	// checks if a settlement can be bought this turn
	if (canBuySett()) {
		if (free) {
			++settlements;
			++victoryPoints;
			canBuySettbool = false;
			return true;
		}
		else if (Resources[(int32)EResource::WHEAT] >= 1 && Resources[(int32)EResource::BRICKS] >= 1 && Resources[(int32)EResource::WOOL] >= 1 && Resources[(int32)EResource::WOOD] >= 1) {
			// decrements the recurses from the player inventory and the global inventory
			--Resources[(int32)EResource::WHEAT];
			--AGameManager::gResources[(int32)EResource::WHEAT];

			--Resources[(int32)EResource::WOOD];
			--AGameManager::gResources[(int32)EResource::WOOD];

			--Resources[(int32)EResource::WOOL];
			--AGameManager::gResources[(int32)EResource::WOOL];

			--Resources[(int32)EResource::BRICKS];
			--AGameManager::gResources[(int32)EResource::BRICKS];

			//adds a settlement to the player and a victory point
			++settlements;
			++victoryPoints;
			//prevents a settlement from being bought this turn
			canBuySettbool = false;
			return true;
		}
	}
	return false; 
}

// methods responsible for buying cities,  it is responsible for checking if the player has the resources to buy the cities and if they have already bought one this turn
// returns true if the cities was bought successfully, false otherwise
bool PlayerInventory::upgradeSettlement() {
	if (canUpgrade() && (Resources[(int32)EResource::WHEAT] >= 2 && Resources[(int32)EResource::ORE] >= 3)) {
		// decrements the recurses from the player inventory and the global inventory, the ---- are equivalent to typing the same -- line twice
		----Resources[(int32)EResource::WHEAT]; 
		----AGameManager::gResources[(int32)EResource::WHEAT];

		------Resources[(int32)EResource::ORE]; 
		------AGameManager::gResources[(int32)EResource::ORE];
		// removes a settlement and adds a city and a victory point, as the settlement is now a city 
		--settlements;
		++cities;
		++victoryPoints;
		// prevents a city from being bought this turn
		canUpgradebool = false;
		return true;
	}
	return false;
}

#pragma endregion

// functions that server functionality related to the cards
#pragma region Cards

// returns the number of card of a specific type that this player has
// carrdC - the card type to check
// returns the number of cards as an integer
int32 PlayerInventory::getCardCount(ECards cardC) {
	// a simple loop that counts the number of cards of a specific type
	int32 count = 0;
	for (ECards card : hand) {
		if (card == cardC) {
			++count;
		}
	}
	return count;
}

// removes one victory point from the player
void PlayerInventory::removeVictoryPoint() {
	--victoryPoints;
}

// attempts to draw a card from the deck
// returns true if the card was drawn successfully, false otherwise
bool PlayerInventory::drawCard() {
	// this check is necessary as drawing from an empty deck results in a crash
	if (AGameManager::Gdeck.Num() == 0) { 
		return false; 
	}
	// checks of the card can be drawn, and if the player has the prerequisite resources
	if (canDrawCardbool && (Resources[(int32)EResource::ORE] >= 1 && Resources[(int32)EResource::WHEAT] >= 1 && Resources[(int32)EResource::WOOL] >= 1)) {
		// removes the resources from the player's inventory, as well as the global inventory
		--Resources[(int32)EResource::ORE]; 
		--AGameManager::gResources[(int32)EResource::ORE];

		--Resources[(int32)EResource::WHEAT]; 
		--AGameManager::gResources[(int32)EResource::WHEAT];

		--Resources[(int32)EResource::WOOL]; 
		--AGameManager::gResources[(int32)EResource::WOOL];

		//selects a random index from the deck, and adds the card to the player's hand
		int32 randno = rand() % AGameManager::Gdeck.Num();
		
		//adds the card to the player's hand, and removes it from the deck
		hand.Add((AGameManager::Gdeck)[randno]);

		//sets the unplayable card to the card that was drawn
		unplayable = (AGameManager::Gdeck)[randno];
		AGameManager::Gdeck.RemoveAt(randno);
		
		// engages the lock to prevent the player from drawing another card
		canDrawCardbool = false;
		return true;
	}
	 return false; 
}

// plays a card from the player's hand of a specific type 
// card - the card to be played
// returns true if the card was played successfully, false otherwise
bool PlayerInventory::useCard(ECards card) {
	// checks if a card can be played
	if (canPlayCardbool) {
		// if the card is unplayable, and the player has more than one of the card, the card cannot be played
		if (card == unplayable) {
			// if the player has more than one of the card, the card cannot be played
			if (!moreThanOne(card)) {
				return false;
			}
		}
		// looks for the card in the player's hand
		int32 cardIndex = hand.Find(card);
		// INDEX_NONE is returned if the card is not found, in which case the function returns false
		if (cardIndex != INDEX_NONE) {
			hand.RemoveAt(cardIndex);
			// prevents the player from playing another card
			canPlayCardbool = false;
			return true;
		}
	}
	
	return false;
}

// checks the player has more than one of a specific card
// card - the card to check
// returns true if the player has more than one of the card, false otherwise
bool PlayerInventory::moreThanOne(ECards card){
	int count = 0;
	// this is done through a simple for each loop that increases a counter every time the card is found
	for (ECards c : hand) {
		if (c == card) { 
			count; 
		} 
	}
	// if the counter is greater than or equal to 2, the player has more than one of the card
	return count>=2;
}

#pragma endregion

// methods responsible for managing the inventory of the player
#pragma region Inventory Management

// gets the number of knights a player has
// returns the number of knights as an integer
int32 PlayerInventory::getKnights() {
	return knights;
}

// adds a knight to the player
void PlayerInventory::addKnight() {
	++knights;
}

// gets the victory points of the player
// returns the victory points of the player as in integer
int32 PlayerInventory::getVP() {
	return victoryPoints;
}

// gets the number of roads a player has
// returns the number of roads as an integer
int32 PlayerInventory::getRoads() {
	return roads;
}

// adds one victory point to the player
void PlayerInventory::addVictoryPoint() {
	++victoryPoints;
}

// removes half of the player's resources, rounded down
void PlayerInventory::removeHalf() {
	// if the total is less than 8, the player does not have enough resources to remove half
	if (total() < 8) {
		return; 
	}
	//each resource is removed one at a time, until half of the player's resources have been removed
	for (int8 i = 0; i < total() / 2; ++i) { 
		removeOneRand(); 
	}
}

// returns the total number of resources the player has, useful for knowing how many resources to remove when a 7 is rolled
// returns the total number of resources as an integer
int32 PlayerInventory::total() { 
	return Resources[0] + Resources[1] + Resources[2] + Resources[3] + Resources[4]; 
}

// removes one of each resource from the player
// returns the type of resource that was removed
EResource PlayerInventory::removeOneRand(){
	// this is done through the use of a label as i have avoided using while loops where possible, since if not implemented correctly they can cause infinite loops which freeze the game
rereoll:
	// if the player has no resources to give, the NONE resource is returned as no resource was removed or can be removed
	if (Resources[(int32)EResource::WHEAT] == 0 && Resources[(int32)EResource::BRICKS] == 0 && Resources[(int32)EResource::ORE] == 0 && Resources[(int32)EResource::WOOD] == 0 && Resources[(int32)EResource::WOOL] == 0) { 
		return EResource::NONE;
	}
	// rolls to remove a random resource
	int randomResource = rand() % 5;
	// if there is no resource of that type to remove, the label is used to re-roll
	if (Resources[randomResource] == 0) { 
		goto rereoll; 
	}
	// a resource is removed from the player's inventory, and the global inventory
	--Resources[randomResource]; 
	--AGameManager::gResources[randomResource];
	// the resource that was removed is returned as in integer cats into an EResource
	return (EResource)randomResource;
}

// adds a resource to the player's inventory
// resource - the resource to be added	
// returns true if the resource was added successfully, false otherwise
bool PlayerInventory::addResource(EResource resource) {
	// checks if the "bank" has enough resources to give the player
	if (AGameManager::gResources[(int32)resource] < 18) { 
		// both the player and the global inventory are updated
		++Resources[(int32)resource]; 
		++AGameManager::gResources[(int32)resource]; 
		return true; 
	}
	return false;
}

// removes a resource from the player's inventory
// resource - the resource to be removed
// returns true if the resource was removed successfully, false otherwise
bool PlayerInventory::removeResource(EResource resource) {
	// checks if the player has any of the resource to remove
	if (Resources[(int32)resource] > 0) { 
		// both the player and the global inventory are updated
		--Resources[(int32)resource]; 
		--AGameManager::gResources[(int32)resource]; 
		return true; 
	}
	return false;
}

// gets the number of a specific resource the player has
// resource - the resource to check
// returns the number of the resource as an integer
int32 PlayerInventory::getResource(EResource resource) {
	return Resources[(int32)resource];
}

// resets the locks on the player inventory
void PlayerInventory::reset() {
	canBuyRoadbool = true;
	canBuySettbool = true;
	canUpgradebool = true;
	canDrawCardbool = true;
	canPlayCardbool = true;
	// resets the unplayable card to none
	unplayable = ECards::NONE;
}

#pragma endregion


 