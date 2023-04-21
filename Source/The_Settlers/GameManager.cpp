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
		invs.Add(new PlayerInventory((EPlayer)(i + 1), resources/5));
	}
}

// Called when the game starts or when spawned
void AGameManager::BeginPlay() {
	StartTurn();
}

//function returns true is the game win condition is met (10 victory points)
//this function is ran on tick
bool AGameManager::END_GAME() {
	return getPlayer(CurrentPlayer)->getVP() > 9;
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
		// the dice is saved into the dice variable that will be displayed on the hud
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
	//locks reset at the end of the turn (these are for the player inventories, things such as can draw, and can build road etc.)
	refreshAll();
	//the player turn gets incremented
	int32 CurrentPlayerInt = (int32)CurrentPlayer;
	CurrentPlayerInt++;
	if (CurrentPlayerInt > (int32)EPlayer::PLAYER4) {
		CurrentPlayerInt = 1;
	}
	CurrentPlayer = (EPlayer)CurrentPlayerInt;
	//this is the special case where the first 8 turs are played in a specific order
	if (globalTurn<9) {
		if (globalTurn == 1) { CurrentPlayer = EPlayer::PLAYER2; }
		if (globalTurn == 2) { CurrentPlayer = EPlayer::PLAYER3; }
		if (globalTurn == 3) { CurrentPlayer = EPlayer::PLAYER4; }
		if (globalTurn == 4) { CurrentPlayer = EPlayer::PLAYER4; }
		if (globalTurn == 5) { CurrentPlayer = EPlayer::PLAYER3; }
		if (globalTurn == 6) { CurrentPlayer = EPlayer::PLAYER2; }
		if (globalTurn == 7) { CurrentPlayer = EPlayer::PLAYER1; }
		if (globalTurn == 8) { CurrentPlayer = EPlayer::PLAYER1; }
		// this is done to force the player to build the roads and settlements, as if it is not done in the first 8 turns, the player will not be able to build any more and get any resources which ruins the game
		buyRandomSett(EPlayer::PLAYER1);
		buyRandomRoad(EPlayer::PLAYER1);
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



#pragma endregion

#pragma region Bot Actions

void AGameManager::startBot() {
	GetWorldTimerManager().SetTimer(botTimeHandle, this, &AGameManager::endBot, ((rand() % botSpeed) + 1), false);
}

void AGameManager::endBot() {
	if (botAction()) { startBot(); }
	else {
		SkipTurn();
	}
}

bool AGameManager::botAction() {
	if (dice == 7) { AThief::thief->moveThief(AHexTileSpawner::hexManager->GetRandomTile()); }
	if (buyRandomSett(CurrentPlayer)) { return true; }
	if (buyRandomRoad(CurrentPlayer)) { return true; }
	if (upgradeRandomSettlements(CurrentPlayer)) { return true; }
	drawCard();
	if (globalTurn > 8) {
		if (knight(CurrentPlayer)){ AThief::thief->moveThief(AHexTileSpawner::hexManager->GetRandomTile()); return true; }
		if (useCard(CurrentPlayer, ECards::MONOPOLY)) { monopoly((EResource)(rand() % 5)); return true; }
		if (freeRoads(CurrentPlayer)) { buyRandomRoad(CurrentPlayer); buyRandomRoad(CurrentPlayer); return true; }
		if (yearOPlenty(CurrentPlayer)) { return true; }
		if (developmentCard(CurrentPlayer)) { return true; }
	}

	return false;
}

bool AGameManager::buyRandomRoad(EPlayer player) {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoad::StaticClass(), foundActors);
	for (AActor* Actor : foundActors) {
		ARoad* road = Cast<ARoad>(Actor);
		if (road->RoadBuyer(player)) {
			return true;
		}

	}
	return false;
}
//randomly buying buildings
bool  AGameManager::upgradeRandomSettlements(EPlayer player) {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
	ShuffleTArray(foundActors);
	for (AActor* Actor : foundActors) {
		ASettlement* sett = Cast<ASettlement>(Actor);
		if (sett->playerOwner == player && !sett->upgraded) {
			if (sett->Upgrade(player)) {
				return true;
			}
		}
	}
	return false;
}

bool AGameManager::buyRandomSett(EPlayer player) {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASettlement::StaticClass(), foundActors);
	ShuffleTArray(foundActors);
	for (AActor* Actor : foundActors) {
		ASettlement* sett = Cast<ASettlement>(Actor);
		if (sett->SettlementBuyer(player)) {
			return true;
		}

	}
	return false;
}
//needed a way to shuffle arays to make the bot random actions more random
void AGameManager::ShuffleTArray(TArray<AActor*>& MyArray) {
	for (int32 i = 0; i < MyArray.Num(); i++)
	{
		int32 j = FMath::RandRange(0, MyArray.Num() - 1);
		MyArray.Swap(i, j);
	}
}

#pragma endregion

#pragma region Cards 

bool AGameManager::yearOPlenty(EPlayer player) {
	if (getPlayer(player)->useCard(ECards::YEAROFPLENTY)) {
		getPlayer(player)->addResource((EResource)((rand() % 5)));
		getPlayer(player)->addResource((EResource)((rand() % 5)));
		return true;
	}
	return false;
}

//monopoly behaviour called by the widget, can also be called by the bots
bool AGameManager::monopoly(EResource resource) {
	int32 total = 0;
	for (PlayerInventory* inv : invs) {
		total += inv->Resources[(int32)resource];
		inv->Resources[(int32)resource] = 0;
	}
	getPlayer(CurrentPlayer)->Resources[(int32)resource] = total;
	return true;
}

//turns on the roadbuilding lock, called by the widget, can also be called by the bots
bool AGameManager::freeRoads(EPlayer player) {
	if (getPlayer(player)->useCard(ECards::FREEROAD)) {
		roadBuildingLock = true;
		return true;
	}
	return false;
}

//called by the widget, can also be called by the bots
//simply adds a knight to the player if the card is playable and played, also released the thief lock
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

//called by the widget, can also be called by the bots
//simply just adds a victory point to the player if the card is playable and played
bool AGameManager::developmentCard(EPlayer player) {
	if (getPlayer(player)->useCard(ECards::VICTORYPOINT)) {
		getPlayer(player)->addVictoryPoint();
		return true;
	}
	return false;
}


//takes the player with the most knights and compares with the one that already has the largest army, if they are different it adds a victory point to the new player and removes it from the old one
void AGameManager::largestArmy() {
	EPlayer playerVP = EPlayer::NONE;
	int largest = 0;
	for (PlayerInventory* inv : invs) {
		if (inv->getKnights() > largest && inv->getKnights() >= 3) {
			largest = inv->getKnights();
			playerVP = inv->player;
		}
	}
	if (largestArmyPlayer == EPlayer::NONE) {
		if (largest > 2) {
			largestArmyPlayer = playerVP;
		}
		return;
	}
	else if (largest > getPlayer(largestArmyPlayer)->getKnights()) {
		----getPlayer(largestArmyPlayer)->victoryPoints; 
		++++getPlayer(playerVP)->victoryPoints;
		largestArmyPlayer = playerVP;
	}
	//GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, FString::Printf(TEXT("Player with largest army:    %d"), (int32)playerVP));
}

//calculates the player with the most roads
void AGameManager::longestRoad() {
	EPlayer playerVP = EPlayer::NONE;
	int largest = 0;
	for (PlayerInventory* inv : invs) {
		if (inv->getRoads() > largest && inv->getRoads() >= 6) {
			largest = inv->getRoads();
			playerVP = inv->player;
		}
	}
	if (longestRoadPlayer == EPlayer::NONE) {
		if (largest > 2) {
			longestRoadPlayer = playerVP;
		}
		return;
	}
	else if (largest > getPlayer(longestRoadPlayer)->getKnights()) {
		----getPlayer(longestRoadPlayer)->victoryPoints; 
		++++getPlayer(playerVP)->victoryPoints;
		longestRoadPlayer = playerVP;
	}
	//GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, FString::Printf(TEXT("Player with largest army:    %d"), (int32)playerVP));
}

//makes the deck "bucket" based on how many cards of each type are required
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

//uses the card for the player, returns true if the card was used sucessfully
bool AGameManager::useCard(EPlayer player, ECards card) {
	return getPlayer(player)->useCard(card);
}

#pragma endregion

#pragma region Inventory Management

// gets the player inventory from the player enum
PlayerInventory* AGameManager::getPlayer(EPlayer player) {
	for (PlayerInventory* inv : invs) {
		if (inv->player == player) { 
			return inv; 
		}
	}
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
			//
			for (int32 i = 0; i < ammount; ++i) {
				getPlayer(player)->removeResource(resource1);
			}										   
			
			getPlayer(player)->addResource(resource2);
			return true;
		}
	}
	return false;
}

// debug 
void AGameManager::addKnight() {
	getPlayer(CurrentPlayer)->addVictoryPoint();
}

// releases the locks on the 
void AGameManager::refreshAll() { 
	for (PlayerInventory* inv : invs) { 
		inv->reset(); 
	} 
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