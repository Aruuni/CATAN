#include "GameManager.h"
#include "PlayerInventory.h"
#include "HexTileSpawner.h"
#include "TimerManager.h"
#include "Thief.h"
#include <Kismet/GameplayStatics.h>

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

#pragma region Turn Mechanics 

void AGameManager::StartTurn() {
	
	int dice1 = rand() % 6 + 1;
	int dice2 = rand() % 6 + 1;
	
	if (globalTurn > 8 ) {
		if (AHexTileSpawner::hexManager->DiceRolled(dice1 + dice2)) {
			stealAll();
			thiefLock = true;
		}
		dice = dice1 + dice2;
	}
	
	if (CurrentPlayer != EPlayer::PLAYER1) {
		startBot();

	}
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &AGameManager::EndTurn, TurnDuration, false);
}

void AGameManager::EndTurn() {
	//these get calculated at the end
	if (thiefLock) {
		stealAll();
	}
	largestArmy();
	longestRoad();
	//locks reset at the end of the turn
	refreshAll();
	//the player turn gets incremented
	int32 CurrentPlayerInt = (int32)CurrentPlayer;
	CurrentPlayerInt++;
	if (CurrentPlayerInt > (int32)EPlayer::PLAYER4) {
		CurrentPlayerInt = 1;
	}
	CurrentPlayer = (EPlayer)CurrentPlayerInt;
	if (globalTurn<9) {
		if (globalTurn == 1) { CurrentPlayer = EPlayer::PLAYER2; }
		if (globalTurn == 2) { CurrentPlayer = EPlayer::PLAYER3; }
		if (globalTurn == 3) { CurrentPlayer = EPlayer::PLAYER4; }
		if (globalTurn == 4) { CurrentPlayer = EPlayer::PLAYER4; }
		if (globalTurn == 5) { CurrentPlayer = EPlayer::PLAYER3; }
		if (globalTurn == 6) { CurrentPlayer = EPlayer::PLAYER2; }
		if (globalTurn == 7) { CurrentPlayer = EPlayer::PLAYER1; }
		if (globalTurn == 8) { CurrentPlayer = EPlayer::PLAYER1; }

	}
	if (globalTurn == 1 || globalTurn == 8) {
		buyRandomSett(EPlayer::PLAYER1);
		buyRandomRoad(EPlayer::PLAYER1);
	}
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
		if (useCard(CurrentPlayer, ECards::KNIGHT)) { knight(CurrentPlayer); AThief::thief->moveThief(AHexTileSpawner::hexManager->GetRandomTile()); return true; }
		if (useCard(CurrentPlayer, ECards::MONOPOLY)) { monopoly((EResource)(rand() % 5)); return true; }
		if (useCard(CurrentPlayer, ECards::FREEROAD)) { buyRandomRoad(CurrentPlayer); buyRandomRoad(CurrentPlayer); return true; }
		if (useCard(CurrentPlayer, ECards::YEAROFPLENTY)) { yearOPlenty(); return true; }
		if (useCard(CurrentPlayer, ECards::VICTORYPOINT)) { developmentCard(CurrentPlayer); return true; }
	}
	return false;
}

bool AGameManager::buyRandomRoad(EPlayer player) {
	if (player == EPlayer::PLAYER1) { return false; }
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
	if (player == EPlayer::PLAYER1) { return false; }
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
	if (player == EPlayer::PLAYER1) { return false; }
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

#pragma region cards 

void AGameManager::yearOPlenty() {
	getPlayer(CurrentPlayer)->addResource((EResource)((rand() % 5)));
	getPlayer(CurrentPlayer)->addResource((EResource)((rand() % 5)));
}

//monopoly behaviour called by the widget, can also be called by the bots
void AGameManager::monopoly(EResource resource) {
	int32 total = 0;
	for (PlayerInventory* inv : invs) {
		total += inv->Resources[(int32)resource];
		inv->Resources[(int32)resource] = 0;
	}
	getPlayer(CurrentPlayer)->Resources[(int32)resource] = total;
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
	if (playerVP != largestArmyPlayer) {
		if (largestArmyPlayer != EPlayer::NONE) { ----getPlayer(largestArmyPlayer)->victoryPoints; }
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
		if (inv->roads > largest && inv->roads >= 6) {
			largest = inv->roads;
			playerVP = inv->player;
		}
	}
	if (playerVP != longestRoadPlayer) {
		if (longestRoadPlayer != EPlayer::NONE) { ----getPlayer(longestRoadPlayer)->victoryPoints; }
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

PlayerInventory* AGameManager::getPlayer(EPlayer player) {
	for (PlayerInventory* inv : invs) {
		if (inv->player == player) { return inv; }
	}
	return nullptr;
}

//checks if there are enough resources to give and recieve and removes the resources from the player and gives them away
//resource1 is the x4 and resource2 is the x1. this is called by the blueprint once settlement adjacency has been established
//returns true/false if the trade was successful
bool AGameManager::shipTrade(EPlayer player, EResource resource1, EResource resource2, int32 ammount) {
	if (gResources[(int32)resource2] < 18) {
		if (getPlayer(player)->Resources[(int32)resource1] > ammount) {
			//manually, i know, who cares
			for (int32 i = 0; i < ammount; ++i) {
				getPlayer(player)->removeResource(resource1);
			}										   
			
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