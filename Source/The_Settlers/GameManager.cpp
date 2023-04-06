#include "GameManager.h"
#include "HexTileSpawner.h"
#include "TimerManager.h"
#include <Kismet/GameplayStatics.h>

AGameManager::AGameManager() {

	this->Gdeck = deckMaker(14, 6, 2, 2, 1);
	TArray<int32*> pArray;
	for (int32 in : gResources) { pArray.Add(&in); }
	for (int8 i = 0; i < 4; ++i) {
		invs.Add(new PlayerInventory((EPlayer)(i + 1), pArray, &Gdeck));
	}
}
// Called when the game starts or when spawned
void AGameManager::BeginPlay() {
	CurrentPlayer = EPlayer::PLAYER1;
	this->gameGlobal = this;
	StartTurn();
}

#pragma region Turn Mechanics 

void AGameManager::StartTurn() {
	FString message = FString::Printf(TEXT("Turn of player: %d"), (int32)CurrentPlayer);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Yellow, message);

	int dice1 = rand() % 6 + 1;
	int dice2 = rand() % 6 + 1;

	FString roll2 = FString::Printf(TEXT("Dice  rolls:    %d"), dice2 + dice1);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, roll2);

	if (AHexTileSpawner::hexManager->DiceRolled(dice1+dice2) == true) {
		thiefLock = true;
	}
	resOut();
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &AGameManager::EndTurn, TurnDuration, false);

}

void AGameManager::EndTurn() {

	
	if (thiefLock) { stealAll(); }
	thiefLock = false;
	if (monopolyLock) { monopoly_AUTO(); }
	monopolyLock = false;
	if (yearOPlentyLock) { yearOPlenty_AUTO(); }
	yearOPlentyLock = false;
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
	getPlayer(CurrentPlayer)->Resources[(int32)resource] = gResources[(int32)resource];
	for (PlayerInventory* inv : invs) {
		if (inv->player == CurrentPlayer) {
			continue;
		}
		inv->Resources[(int32)resource] = 0;
	}
}
void AGameManager::monopoly_AUTO() {
	int randomNumber = rand() % 5;
	getPlayer(CurrentPlayer)->Resources[(int32)randomNumber] = gResources[(int32)randomNumber ];
	for (PlayerInventory* inv : invs) {
		if (inv->player == CurrentPlayer) {
			continue;
		}
		inv->Resources[(int32)randomNumber + 1] = 0;
	}
}

void AGameManager::freeRoads() {

}
//minor rewrite
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

#pragma endregion

#pragma region Inventory Management

 
void AGameManager::refreshAll() { for (PlayerInventory* inv : invs) { inv->reset(); } }

void AGameManager::stealAll() { for (PlayerInventory* inv : invs) { inv->removeHalf(); } }


#pragma endregion



#pragma region HUD

FString AGameManager::getResourceHUD(EPlayer player, EResource resource) {
	return FString::FromInt(getPlayer(player)->getResource(resource));
}

void AGameManager::resOut() {
	for (PlayerInventory* pinv : invs) {
		pinv->resOut();
	}
}

#pragma endregion 