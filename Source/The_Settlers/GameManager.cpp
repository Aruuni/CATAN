#include "GameManager.h"
#include "PlayerInventory.h"
#include "HexTileSpawner.h"
#include "TimerManager.h"
#include <Kismet/GameplayStatics.h>

AGameManager::AGameManager() {
	CurrentPlayer = EPlayer::PLAYER1;
	gameGlobal = this;
	Gdeck = deckMaker(14, 6, 2, 2, 1);
	for (int8 i = 0; i < 4; ++i) {
		invs.Add(new PlayerInventory((EPlayer)(i + 1)));
	}
}
// Called when the game starts or when spawned
void AGameManager::BeginPlay() {
	StartTurn();
}

#pragma region Turn Mechanics 

void AGameManager::StartTurn() {
	int dice1 = rand() % 6 + 1;
	int dice2 = rand() % 6 + 1;

	if (AHexTileSpawner::hexManager->DiceRolled(dice1+dice2)) {
		thiefLock = true;
	}
	dice = dice1 + dice2;
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &AGameManager::EndTurn, TurnDuration, false);
}

void AGameManager::EndTurn() {
	//automatic behaviour for the bots
	if (thiefLock) { stealAll(); }
	thiefLock = false;

	if (monopolyLock) { monopoly((EResource) (rand() % 5)); }
	monopolyLock = false;

	if (yearOPlentyLock) { yearOPlenty(); }
	yearOPlentyLock = false;

	if (roadBuildingLock) { freeRoadsCount = 0; roadBuildingLock = false; }
	
	//these get calculated at the end
	largestArmy();
	//longestRoad();
	//locks reset at the end of the turn
	refreshAll();
	//the player turn gets incremented
	int32 CurrentPlayerInt = (int32)CurrentPlayer;
	CurrentPlayerInt++;
	if (CurrentPlayerInt > (int32)EPlayer::PLAYER4) {
		CurrentPlayerInt = 1;
	}
	CurrentPlayer = (EPlayer)CurrentPlayerInt;
	globalTurn++;
	StartTurn();
}

void AGameManager::SkipTurn() {
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	EndTurn();
}

EPlayer AGameManager::getCurrentPlayer() {
	return CurrentPlayer;
}

#pragma endregion

#pragma region cards 

void AGameManager::yearOPlenty() {
	int rcount = 0;
	int fcount = 0;
	while (rcount != 2|| fcount == 5) {
		if (getPlayer(CurrentPlayer)->addResource((EResource)((rand() % 5) + 1))) { ++rcount; }
		else { ++fcount; }
	}
}

//monopoly behaviour called by the widget, can also be called by the bots
void AGameManager::monopoly(EResource resource) {
	for (PlayerInventory* inv : invs) {
		inv->Resources[(int32)resource] = 0;
	}
	getPlayer(CurrentPlayer)->Resources[(int32)resource] = gResources[(int32)resource];
	monopolyLock = false;
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
		largestArmy();
		return true;
	}
	return false;
}

//called by the widget, can also be called by the bots
//simply just adds a victory point to the player if the card is playable and played
bool AGameManager::victoryPoint(EPlayer player) {
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
		if (inv->knights > largest && inv->knights >= 3) {
			largest = inv->knights;
			playerVP = inv->player;
		}
	}
	if (playerVP != largestArmyPlayer) {
		if (largestArmyPlayer != EPlayer::NONE) { ----getPlayer(largestArmyPlayer)->victoryPoints; }
		++++getPlayer(playerVP)->victoryPoints;
	}
	//GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, FString::Printf(TEXT("Player with largest army:    %d"), (int32)playerVP));
}


void AGameManager::longestRoad() {
	EPlayer playerVP = EPlayer::NONE;
	int largest = 0;
	for (PlayerInventory* inv : invs) {
		if (inv->roads > largest && inv->roads >= 6) {
			largest = inv->roads;
			playerVP = inv->player;
		}
	}
	if (playerVP != largestArmyPlayer) {
		if (largestArmyPlayer != EPlayer::NONE) { ----getPlayer(largestArmyPlayer)->victoryPoints; }
		++++getPlayer(playerVP)->victoryPoints;
	}
	//GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, FString::Printf(TEXT("Player with largest army:    %d"), (int32)playerVP));
}

//makes the deck "bucket" based on how many cards of each type are required
TArray<ECards> AGameManager::deckMaker(int knight, int vp, int monopoly, int yop, int roads) {
	TArray<ECards> deck;
	for (int8 i = 0; i < knight + vp + monopoly + yop + roads; ++i) {
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

PlayerInventory* AGameManager::getPlayer(EPlayer player) {
	for (PlayerInventory* inv : invs) {
		if (inv->player == player) { return inv; }
	}
	return nullptr;
}

//checks if there are enough resources to give and recieve and removes the resources from the player and gives them away
//resource1 is the x4 and resource2 is the x1. this is called by the blueprint once settlement adjacency has been established
//returns true/false if the trade was successful
bool AGameManager::shipTrade(EPlayer player, EResource resource1, EResource resource2) {
	if (gResources[(int32)resource2] < 18) {
		if (getPlayer(player)->Resources[(int32)resource1] >= 4) {
			//manually, i know, who cares
			getPlayer(player)->removeResource(resource1);
			getPlayer(player)->removeResource(resource1);
			getPlayer(player)->removeResource(resource1);
			getPlayer(player)->removeResource(resource1);
			getPlayer(player)->addResource(resource2);
			return true;
		}
	}
	return false;
}

// removes the locks on the inventories
void AGameManager::refreshAll() { for (PlayerInventory* inv : invs) { inv->reset(); } }

//steals half of the resources from all players, called by the thief
void AGameManager::stealAll() { for (PlayerInventory* inv : invs) { inv->removeHalf(); } }

//checks if there are enough resources to give and recieve and removes the resources from the player and gives them away
//player2 is the initiator of the trade, player1 is the reciever
//resource 2 is the resource the initiator wants and resource 1 is the resource the reciever gets
bool AGameManager::trade(EPlayer player1, EPlayer player2, EResource resource1, EResource resource2) {
	if (getPlayer(player1)->getResource(resource2) > 0 && getPlayer(player2)->getResource(resource1) > 0) {
		getPlayer(player1)->addResource(resource1);
		getPlayer(player1)->removeResource(resource2);
		getPlayer(player2)->addResource(resource2);
		getPlayer(player2)->removeResource(resource1);
		return true;
	}
	return false;
}

//draws a card from the deck and adds it to the players hand, called by the widget "draw card" button
bool AGameManager::drawCard() {
	return getPlayer(CurrentPlayer)->drawCard();
}

//adds a resource of the type to the players inventory
//used for year of plenty widget
bool AGameManager::addResource(EPlayer player, EResource resource) {
	return getPlayer(player)->addResource(resource);
}

#pragma endregion

#pragma region HUD Getters

FString AGameManager::getResourceHUD(EPlayer player, EResource resource) {
	//GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, FString::FromInt(getPlayer(player)->getResource(resource)));

	return FString::FromInt(getPlayer(player)->getResource(resource));
}

int32 AGameManager::knightCount(EPlayer player) {
	return getPlayer(player)->getKnights();
}

int32 AGameManager::getRemainingTime() {
	return (int32)GetWorldTimerManager().GetTimerRemaining(TurnTimerHandle);

}

int32 AGameManager::getDice() {
	return dice;
}

int32 AGameManager::getCardCount(EPlayer player, ECards card) {
	return getPlayer(player)->getCardCount(card);
}

EPlayer AGameManager::getLargestArmyPlayer() {
	return largestArmyPlayer;
}

EPlayer AGameManager::getLongestRoadPlayer() {
	return longestRoadPlayer;
}

int32 AGameManager::getPlayerTurn() {
	return (int32)getCurrentPlayer();
}

int32 AGameManager::getTurnNumber() {
	return globalTurn;
}

int32 AGameManager::getVictoryPoints(EPlayer player) {
	return getPlayer(player)->getVP();
}

#pragma endregion 