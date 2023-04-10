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
	FString turn = FString::Printf(TEXT("Turn : %d"), globalTurn);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Yellow, turn);
	FString message = FString::Printf(TEXT("Turn of player: %d"), (int32)CurrentPlayer);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Yellow, message);


	int dice1 = rand() % 6 + 1;
	int dice2 = rand() % 6 + 1;

	FString roll2 = FString::Printf(TEXT("Dice  rolls:    %d"), dice2 + dice1);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, roll2);

	if (AHexTileSpawner::hexManager->DiceRolled(dice1+dice2)) {
		thiefLock = true;
	}
	//resOut();
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &AGameManager::EndTurn, TurnDuration, false);

}

void AGameManager::EndTurn() {

	if (thiefLock) { stealAll(); }
	thiefLock = false;

	if (monopolyLock) { monopoly((EResource) (rand() % 5)); }
	monopolyLock = false;

	if (yearOPlentyLock) { yearOPlenty_AUTO(); }
	yearOPlentyLock = false;

	if (freeRoadsLock) { freeRoadsCount = 0; freeRoadsLock = false; }

	largestArmy();

	//Next turn 
	refreshAll();
	int32 CurrentPlayerInt = (int32)CurrentPlayer;
	CurrentPlayerInt++;
	if (CurrentPlayerInt >= (int32)EPlayer::PLAYER4) {
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

PlayerInventory* AGameManager::getPlayer(EPlayer player) {
	for (PlayerInventory* inv : invs) {
		if (inv->player == player) { return inv; }
	}
	return nullptr;
}

#pragma region cards 

void AGameManager::yearOPlenty_AUTO() {
	int rcount = 0;
	int fcount = 0;
	while (rcount != 2|| fcount == 5) {
		if (getPlayer(CurrentPlayer)->addResource((EResource)((rand() % 5) + 1))) { ++rcount; }
		else { ++fcount; }
	}
}

void AGameManager::monopoly(EResource resource) {
	for (PlayerInventory* inv : invs) {
		inv->Resources[(int32)resource] = 0;
	}
	getPlayer(CurrentPlayer)->Resources[(int32)resource] = gResources[(int32)resource];
	monopolyLock = false;
}

bool AGameManager::freeRoads(EPlayer player) {
	if (getPlayer(player)->useCard(ECards::FREEROAD)) {
		freeRoadsLock = true;
		return true;
	}
	return false;
}

bool AGameManager::knight(EPlayer player) {
	if (getPlayer(player)->useCard(ECards::KNIGHT)) {
		getPlayer(CurrentPlayer)->addKnight();
		thiefLock = true;
		return true;
	}
	return false;
}

bool AGameManager::victoryPoint(EPlayer player) {
	if (getPlayer(player)->useCard(ECards::VICTORYPOINT)) {
		getPlayer(player)->victoryPoints++;
		return true;
	}
	return false;
}

void AGameManager::largestArmy() {
	EPlayer playerVP = EPlayer::NONE;
	int largest = 0;
	for (PlayerInventory* inv : invs) {
		if (inv->knights > largest) {
			//GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, FString::Printf(TEXT("swapped")));
			largest = inv->knights;
			playerVP = inv->player;
		}
	}
	if (playerVP != largestArmyPlayer) {
		if (largestArmyPlayer != EPlayer::NONE) { ----getPlayer(largestArmyPlayer)->victoryPoints; }
		++++getPlayer(playerVP)->victoryPoints;
	}
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, FString::Printf(TEXT("Player with largest army:    %d"), (int32)playerVP));
}

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

bool AGameManager::useCard(EPlayer player, ECards card) {
	return getPlayer(player)->useCard(card);
}

int32 AGameManager::getCardCount(EPlayer player, ECards card) {
	return getPlayer(player)->getCardCount(card);
}

#pragma endregion

#pragma region Inventory Management

 
void AGameManager::refreshAll() { for (PlayerInventory* inv : invs) { inv->reset(); } }

void AGameManager::stealAll() { for (PlayerInventory* inv : invs) { inv->removeHalf(); } }

bool AGameManager::trade(EPlayer player1, EPlayer player2, EResource resource1, EResource resource2) {
	if (getPlayer(player1)->getResource(resource1) > 0) {
		getPlayer(player1)->removeResource(resource1);
		getPlayer(player2)->addResource(resource2);
		return true;
	}
	return false;

}

bool AGameManager::addResource(EPlayer player, EResource resource) {
	return getPlayer(player)->addResource(resource);
}

#pragma endregion

#pragma region HUD

FString AGameManager::getResourceHUD(EPlayer player, EResource resource) {
	//GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, FString::FromInt(getPlayer(player)->getResource(resource)));

	return FString::FromInt(getPlayer(player)->getResource(resource));
}

void AGameManager::resOut() {
	for (PlayerInventory* pinv : invs) {
		pinv->resOut();
	}
}

#pragma endregion 