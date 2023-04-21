#include "GameManager.h"
#include "PlayerInventory.h"
#include "HexTileSpawner.h"
#include "TimerManager.h"
#include "Thief.h"
#include <Kismet/GameplayStatics.h>

//constructor
AGameManager::AGameManager() {
	CurrentPlayer = EPlayer::PLAYER1;
	gameGlobal = this;
	Gdeck = deckMaker(14, 6, 2, 2, 1);
	for (int8 i = 0; i < 4; ++i) {
		// adds a new player inventory to the array of player inventories
		invs.Add(new PlayerInventory((EPlayer)(i + 1), 0));
	}
}

// Called when the level loads and calls start turn to begin the turn loop
void AGameManager::BeginPlay() {
	CurrentPlayer = EPlayer::PLAYER1;
	StartTurn();
}

#pragma region Turn Mechanics 

//this function is called when a new turn begins, or when the turn ends
void AGameManager::StartTurn() {
	//dice roll is made up of two dice of six sides. The dice roll is used to determine the resources gained by the players
	int dice1 = rand() % 6 + 1;
	int dice2 = rand() % 6 + 1;
	//the dice is rolled only after the 8th turn
	if (globalTurn > 8 ) {
		// this returns true if the dice roll is 7, and so it automatically steals, and engages the thief lock
		if (AHexTileSpawner::hexManager->DiceRolled(dice1 + dice2)) {
			stealAll();
			thiefLock = true;
		}
		// the dice is saved into the dice variable that will be displayed on the HUD
		dice = dice1 + dice2;
	}
	// this started the bot action loop only if the current player is not the actual player
	if (CurrentPlayer != EPlayer::PLAYER1) {
		startBot();
	}
	// this sets the timer for the turn
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &AGameManager::EndTurn, TurnDuration, false);
}

//this function is called when the turn ends
void AGameManager::EndTurn() {
	// the longest road and largest army are calculated at the end of the turn as a fail safe, they are very light functions so overhead is minimal
	largestArmy();
	longestRoad();
	//the settlement and road is automatically placed if the player fails to in order to prevent a soft lock where the player is unable to advance
	if (globalTurn < 9) {
		// this is done to force the player to build the roads and settlements, as if it is not done in the first 8 turns, the player will not be able to build any more and get any resources which ruins the game

		buyRandomSett(EPlayer::PLAYER1);
		buyRandomRoad(EPlayer::PLAYER1);
	}
	//locks reset at the end of the turn (these are for the player inventories, things such as can draw, and can build road etc.), but before the player is incremented as then it would not properly release the locks and will be carried over to the next round
	refreshAll();

	//the player turn gets incremented
	int32 CurrentPlayerInt = (int32)CurrentPlayer;
	CurrentPlayerInt++;
	if (CurrentPlayerInt > (int32)EPlayer::PLAYER4) {
		CurrentPlayerInt = 1;
	}
	CurrentPlayer = (EPlayer)CurrentPlayerInt;

	//this is the special case where the first 8 turns are played in a specific order
	if (globalTurn<9) {
		if (globalTurn == 1) { 
			CurrentPlayer = EPlayer::PLAYER2; 
		}
		if (globalTurn == 2) { 
			CurrentPlayer = EPlayer::PLAYER3; 
		}
		if (globalTurn == 3) { 
			CurrentPlayer = EPlayer::PLAYER4; 
		}
		if (globalTurn == 4) { 
			CurrentPlayer = EPlayer::PLAYER4; 
		}
		if (globalTurn == 5) { 
			CurrentPlayer = EPlayer::PLAYER3; 
		}
		if (globalTurn == 6) { 
			CurrentPlayer = EPlayer::PLAYER2; 
		}
		if (globalTurn == 7) { 
			CurrentPlayer = EPlayer::PLAYER1;	
		}
		if (globalTurn == 8) { 
			CurrentPlayer = EPlayer::PLAYER1; 
		}
	}

	// the global turn is incremented then the next turn is started
	globalTurn++;
	StartTurn();
}

// this is the skip button function, it is called when the skip button is pressed
void AGameManager::SkipTurn() {
	// this stop the current timer to make sure that the next player gets the full turn time
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	// the turn is ended
	EndTurn();
}

//function returns true is the game win condition is met (10 victory points)
//this function is ran on tick
// returns true if the win condition is met, false if it is not
bool AGameManager::END_GAME() {
	return getPlayer(CurrentPlayer)->getVP() > 9;
}

#pragma endregion

// actions done by the bot, it is called when the bot starts its turn, it is done to make the bot actions look more natural, as instant bot actions would look very unnatural to the human player
#pragma region Bot Actions

// starts the bot action loop, it is called when the bot starts its turn this is done to make the bot actions look more natural, as instant bot actions would look very unnatural to the player
void AGameManager::startBot() {
	//sets a random timer between 1 and botSpeed (an integer) seconds
	GetWorldTimerManager().SetTimer(botTimeHandle, this, &AGameManager::endBot, ((rand() % botSpeed) + 1), false);
}

// function called when the startBot() timer ends , it calls the botAction() function as long as it can then skips the turn
void AGameManager::endBot() {
	// as long as a bot action is done, the bot action loop is restarted
	if (botAction()) { startBot(); }
	// if bot action returns false, it means that no action / other action could be done, so the bot turn is ended by calling the skip turn function
	else {
		SkipTurn();
	}
}

// this is the bot action function, it tries to do all the actions in the order they are written, if it cannot do any of them, or when it does one, it returns true, and the bot action loop is restarted
// it prioritizes building over other actions as earning resources is the most important thing in the game
// it attempts to do all the actions one by one, if a function has already been done, it will not do it again as its locks will prevent it and attempt to do the next action
// returns true if it does an action, false if it does not
bool AGameManager::botAction() {
	if (dice == 7) { AThief::thief->moveThief(AHexTileSpawner::hexManager->GetRandomTile()); }
	if (buyRandomSett(CurrentPlayer)) {
		return true; 
	}
	if (buyRandomRoad(CurrentPlayer)) { 
		return true; 
	}

	// it will attempt to buy a development card if it has not already done so
	drawCard();

	// moves the thief to a random tile if the dice roll is 7
	if (knight(CurrentPlayer)){ 
		AThief::thief->moveThief(AHexTileSpawner::hexManager->GetRandomTile()); 
		return true; }
	// calls the monopoly function on a random resource
	if (monopoly(CurrentPlayer, (EResource)(rand() % 5))) { 
		return true; 
	}
	// calls the road building function, then buys two random roads
	if (freeRoads(CurrentPlayer)) { 
		buyRandomRoad(CurrentPlayer); buyRandomRoad(CurrentPlayer); 
		return true; 
	}
	// calls the year of plenty function
	if (yearOPlenty(CurrentPlayer)) { 
		return true; 
	}
	// calls the development card function
	if (developmentCard(CurrentPlayer)) { 
		return true; 
	}
	
	//upgrading settlements is the least important
	if (upgradeRandomSettlements(CurrentPlayer)) { 
		return true; 
	}

	return false;
}

// buys a random road  by looping through all the roads and calling the roadBuyer function on them
// player - the player that is buying the road
// returns true if a road is bought, false if not
bool AGameManager::buyRandomRoad(EPlayer player) {
	TArray<AActor*> foundActors;
	//gets all the roads in the world
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoad::StaticClass(), foundActors);
	//shuffles the array so that the roads are not bought in the same order every time
	ShuffleTArray(foundActors);
	for (AActor* Actor : foundActors) {
		//casts the actor to a road
		ARoad* road = Cast<ARoad>(Actor);
		// attempts to buy the road, if the road is not purchasable under the rules of the game, it will return false with no side effects so calling it on all roads is safe
		if (road->RoadBuyer(player)) {
			return true;
		}

	}
	return false;
}

// buys a random settlement  by looping through all the settlement and calling the SettlementBuyer function on them
// player - the player that is buying the settlement
// returns true if a settlement is bought, false if not
bool AGameManager::buyRandomSett(EPlayer player) {
	TArray<AActor*> foundActors;
	//gets all the settlements in the world
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
	ShuffleTArray(foundActors);
	for (AActor* Actor : foundActors) {
		ASettlement* sett = Cast<ASettlement>(Actor);
		// attempts to buy the settlement, if the settlement is not purchasable under the rules of the game, it will return false with no side effects so calling it on all settlements is safe
		if (sett->SettlementBuyer(player)) {
			return true;
		}
	}
	return false;
}

// upgrades a random settlement by looping through all the settlement and calling the Upgrade function on them
// player - the player that is buying the settlement
// returns true if a settlement is upgraded to a city, false if not
bool  AGameManager::upgradeRandomSettlements(EPlayer player) {
	TArray<AActor*> foundActors;
	//gets all the settlements in the world
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
	//shuffles the array so that the settlements are not bought in the same order every time
	ShuffleTArray(foundActors);
	for (AActor* Actor : foundActors) {
		//casts the actor to a settlement
		ASettlement* sett = Cast<ASettlement>(Actor);
		// attempts to upgrade the settlement, if the settlement is not purchasable under the rules of the game, it will return false with no side effects so calling it on all settlements is safe
		if (sett->playerOwner == player && !sett->upgraded) {
			if (sett->Upgrade(player)) {
				return true;
			}
		}
	}
	return false;
}

// needed a way to shuffle arrays to make the bot random actions more random, it is a linear function that swaps each element with another random element
// MyArray - a reference to the array to be shuffled 
// does not return the array, as it is a reference to the array, so it is changed in place
void AGameManager::ShuffleTArray(TArray<AActor*>& MyArray) {
	for (int32 i = 0; i < MyArray.Num(); i++) {
		// randomly swap the element with another one 
		int32 j = FMath::RandRange(0, MyArray.Num() - 1);
		MyArray.Swap(i, j);
	}
}

#pragma endregion

// functions related to the card functionality 
#pragma region Cards 

// the year of plenty behavior called by the bots, the player will add resources that they choose from the widget so this is not used by players
// player - player using the card
// returns true if the card was played successfully
bool AGameManager::yearOPlenty(EPlayer player) {
	if (getPlayer(player)->useCard(ECards::YEAROFPLENTY)) {
		// adds two random resources to the player
		getPlayer(player)->addResource((EResource)((rand() % 5)));
		getPlayer(player)->addResource((EResource)((rand() % 5)));
		return true;
	}
	return false;
}

// monopoly behavior called by the widget, can also be called by the bots
// takes all the resources of a type from all players and gives them to the current player
// it does this by setting all the other players resources to 0 and then setting thee current players resources to the total of the specific gResource
// player - player using the card
// resource - the type of resource to monopolize
// returns true if the card was played successfully
bool AGameManager::monopoly(EPlayer player, EResource resource) {
	if (useCard(player, ECards::MONOPOLY)) {
		//zeros every player as the player that played the card will get assigned the gResource regardless
		for (PlayerInventory* inv : invs) {
			inv->Resources[(int32)resource] = 0;
		}
		//assigns the player the gResource
		getPlayer(player)->Resources[(int32)resource] = gResources[(int32)resource];
		return true;
	}
	return false;
}

// turns on the road building lock, called by the widget, can also be called by the bots
// player - using the card
// returns true if the card was played successfully
bool AGameManager::freeRoads(EPlayer player) {
	if (getPlayer(player)->useCard(ECards::FREEROAD)) {
		roadBuildingLock = true;
		return true;
	}
	return false;
}

// called by the widget, can also be called by the bots
// simply adds a knight to the player if the card is playable and played, also released the thief lock and steals all the resources 
// player - that plays the card
// returns true if the card was played successfully
bool AGameManager::knight(EPlayer player) {
	if (getPlayer(player)->useCard(ECards::KNIGHT)) {
		getPlayer(CurrentPlayer)->addKnight();
		thiefLock = true;
		stealAll();
		largestArmy();
		return true;
	}
	return false;
}

// called by the widget, can also be called by the bots
// simply just adds a victory point to the player if the card is playable and played
// player - that plays the card
// returns true if the card was played successfully
bool AGameManager::developmentCard(EPlayer player) {
	//calls the use card function to validate if the card can be played
	if (getPlayer(player)->useCard(ECards::VICTORYPOINT)) {
		getPlayer(player)->addVictoryPoint();
		return true;
	}
	return false;
}

// takes the player with the most knights and compares with the one that already has the largest army, if they are different it adds a victory point to the new player and removes it from the old one
void AGameManager::largestArmy() {
	EPlayer playerVP = EPlayer::NONE;
	int largest = 0;
	// loops through the players and finds the one with the most knights
	//a check for the NONE player because the player NONE does not exist
	for (PlayerInventory* inv : invs) {
		if (inv->getKnights() > largest && inv->getKnights() >= 3) {
			largest = inv->getKnights();
			playerVP = inv->player;
		}
	}
	//if there is no player with the most knights and the largest knights so far are more than 2, the largest army player is set to the player with the most knights
	if (largestArmyPlayer == EPlayer::NONE) {
		if (largest > 2) {
			largestArmyPlayer = playerVP;
		}
		return;
	}
	//if the largest army detected so far is more then the player with the most knights, the largest army player is set to the player with the most knights
	else if (largest > getPlayer(largestArmyPlayer)->getKnights()) {
		//2 victory points are added and removed from the players accordingly
		----getPlayer(largestArmyPlayer)->victoryPoints; 
		++++getPlayer(playerVP)->victoryPoints;
		//finally the new largest army player is set
		largestArmyPlayer = playerVP;
	}
}

// calculates the player with the most roads
// this function is played whenever a road is built 
void AGameManager::longestRoad() {
	EPlayer playerVP = EPlayer::NONE;
	int largest = 0;
	//loops through the players and finds the one with the most roads
	for (PlayerInventory* inv : invs) {
		if (inv->getRoads() > largest && inv->getRoads() >= 6) {
			largest = inv->getRoads();
			playerVP = inv->player;
		}
	}
	//if there is no player with the most roads and the longest roads so far are more than 6, the longest road player is set to the player with the most roads so far
	//a check for the NONE player because the player NONE does not exist
	if (longestRoadPlayer == EPlayer::NONE) {
		if (largest > 6) {
			longestRoadPlayer = playerVP;
		}
		return;
	}
	else if (largest > getPlayer(longestRoadPlayer)->getKnights()) {
		// two victory points are added and removed from the players accordingly
		----getPlayer(longestRoadPlayer)->victoryPoints; 
		++++getPlayer(playerVP)->victoryPoints;
		//finally the new longest road player is set
		longestRoadPlayer = playerVP;
	}
	//GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, FString::Printf(TEXT("Player with largest army:    %d"), (int32)playerVP));
}

// makes the deck "bucket" based on how many cards of each type are required
// knight - number of knight cards
// vp - number of victory point cards
// monopoly - number of monopoly cards
// yop - number of year of plenty cards
// roads - number of free road cards
// returns a TArray of ECards
TArray<ECards> AGameManager::deckMaker(int knight, int vp, int monopoly, int yop, int roads) {
	TArray<ECards> deck;
	for (int8 i = 0; i < knight + vp + monopoly + yop + roads+1; ++i) {
		if (i < knight + 1) { deck.Add(ECards::KNIGHT); continue; }
		if (i < knight + vp + 1) { deck.Add(ECards::VICTORYPOINT); continue; }
		if (i < knight + vp + monopoly + 1) { deck.Add(ECards::MONOPOLY); continue; }
		if (i < knight + vp + monopoly + yop + 1) { deck.Add(ECards::YEAROFPLENTY); continue; }
		if (i < knight + vp + monopoly + yop + roads + 1) { deck.Add(ECards::FREEROAD); continue; }
	}
	return deck;
}

// uses the card for the player. this function is used to validate whether or not a specific player has a card can play it, 
// the player that is using the card
// the card that is being used
// returns true if the card was used successfully
bool AGameManager::useCard(EPlayer player, ECards card) {
	return getPlayer(player)->useCard(card);
}

#pragma endregion

// functions to manage the player inventories
#pragma region Inventory Management

// gets the player inventory from the player Enum
PlayerInventory* AGameManager::getPlayer(EPlayer player) {
	for (PlayerInventory* inv : invs) {
		if (inv->player == player) { 
			return inv; 
		}
	}
	// will never actually happen, but otherwise the compiler complains that not all paths return a value
	return nullptr;
}

// checks if there are enough resources to give and receive and removes the resources from the player and gives them away
// // this is called by the blueprint once settlement adjacency has been established
// resource1 is the resource to be given to the ship (between 2 and 3) 
// resource2 is the x1 that the player receives in the trade. 
// returns true/false if the trade was successful
bool AGameManager::shipTrade(EPlayer player, EResource resource1, EResource resource2, int32 ammount) {
	//since this trade type generates a resource, it needs to be checked if the "bank" has enough resources to give away
	if (gResources[(int32)resource2] < 18) {
		//checks if the player has enough resources to give away
		if (getResource(player, resource2) > ammount) {
			//removes as many resources as the player needs to give away, generated by the ship tile
			for (int32 i = 0; i < ammount; ++i) {
				getPlayer(player)->removeResource(resource1);
			}										   
			getPlayer(player)->addResource(resource2);
			return true;
		}
	}
	return false;
}

// releases the locks on the current player
void AGameManager::refreshAll() { 
	getPlayer(CurrentPlayer)->reset();
}

//s teals half of the resources from all players, called by the thief
void AGameManager::stealAll() { 
	for (PlayerInventory* inv : invs) { 
		inv->removeHalf(); 
	} 
}

// checks if there are enough resources to give and receive and removes the resources from the player and gives them away
// player2 is the initiator of the trade, player1 is the receiver
// resource 2 is the resource the initiator wants and resource 1 is the resource the receiver gets
// returns true/false if the trade was successful/unsuccessful
bool AGameManager::trade(EPlayer player1, EPlayer player2, EResource resource1, EResource resource2) {
	// checks to see if the players have the resources, the global resource array is not checked because no new resource is being created, just moved from one player to another
	if (getPlayer(player1)->getResource(resource2) > 0 && getPlayer(player2)->getResource(resource1) > 0) {
		//adds and removes the resources traded for both players
		getPlayer(player1)->addResource(resource1);
		getPlayer(player1)->removeResource(resource2);
		getPlayer(player2)->addResource(resource2);
		getPlayer(player2)->removeResource(resource1);
		return true;
	}
	return false;
}

//draws a card from the deck and adds it to the players hand, called by the widget "draw card" button
// returns true if the card was drawn
bool AGameManager::drawCard() {
	return getPlayer(CurrentPlayer)->drawCard();
}

// adds a resource of the type to the players inventory
// used for year of plenty widget and auto play of the card
// the player who played the card
// the resource to add
// returns true if the resource was added
bool AGameManager::addResource(EPlayer player, EResource resource) {
	return getPlayer(player)->addResource(resource);
}

#pragma endregion

// these are getters for the to display different statistics on the HUD, they are called by functions bound to the HUD elements
#pragma region HUD Getters

// gets the resource count for the HUD to display per player per resource type
// also used to check if the player has enough resources for other actions such as trading
int32 AGameManager::getResource(EPlayer player, EResource resource) {
	return getPlayer(player)->getResource(resource);
}

// used to display the number of knights a player has on the HUD
int32 AGameManager::knightCount(EPlayer player) {
	return getPlayer(player)->getKnights();
}

// this is used to show the remaining time of the current turn
int32 AGameManager::getRemainingTime() {
	return (int32)GetWorldTimerManager().GetTimerRemaining(TurnTimerHandle);

}

// gets the current dice roll for the HUD to display
int32 AGameManager::getDice() {
	return dice;
}

// gets the number of cards of a certain type for the HUD so the player can see how many cards they have per card type
int32 AGameManager::getCardCount(EPlayer player, ECards card) {
	return getPlayer(player)->getCardCount(card);
}

// gets the player with the largest army to display it on the HUD
EPlayer AGameManager::getLargestArmyPlayer() {
	return largestArmyPlayer;
}

// gets the player with the longest road to display it on the HUD
EPlayer AGameManager::getLongestRoadPlayer() {
	return longestRoadPlayer;
}

// this is to get the turn number for the HUD for the player to see what turn it is
int32 AGameManager::getTurnNumber() {
	return globalTurn;
}

// this is a getter for the victory points of a player, they are called in 4 different getText functions in the HUD blueprint
int32 AGameManager::getVictoryPoints(EPlayer player) {
	return getPlayer(player)->getVP();
}

// this is a function used by the HUD to get the current player in the event graph of blueprints
EPlayer AGameManager::getCurrentPlayer() {
	return CurrentPlayer;
}

#pragma endregion 