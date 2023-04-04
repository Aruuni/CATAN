#include "GameManager.h"
#include "HexTileSpawner.h"
#include "TimerManager.h"
#include <Kismet/GameplayStatics.h>

AGameManager::AGameManager() {}
// Called when the game starts or when spawned
void AGameManager::BeginPlay() {
	CurrentPlayer = EPlayer::PLAYER1;
	this->gameGlobal = this;
	players = new Players(0,0,0,0,0);
	StartTurn();
}

#pragma region Turn Mechanics --------- NOT DONE  -----------

void AGameManager::StartTurn() {
	thiefLock = false;
	FString message = FString::Printf(TEXT("Turn of player: %d"), (int32)CurrentPlayer);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Yellow, message);

	int dice1 = rand() % 6 + 1;
	int dice2 = rand() % 6 + 1;

	FString roll2 = FString::Printf(TEXT("Dice  rolls:    %d"), dice2 + dice1);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, roll2);

	if (AHexTileSpawner::hexManager->DiceRolled(7) == true) {
		thiefLock = false;
	}
	players->resOut();
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &AGameManager::EndTurn, TurnDuration, false);

}

void AGameManager::EndTurn() {
	// Cycle through the players
	int32 CurrentPlayerInt = (int32)CurrentPlayer;
	CurrentPlayerInt++;
	if (CurrentPlayerInt >= (int32)EPlayer::PLAYER4){
		CurrentPlayerInt = 1;
	}
	CurrentPlayer = (EPlayer)CurrentPlayerInt;
	globalTurn++;
	players->refreshAll();
	// Start the next turn
	thiefLock = true;
	StartTurn();
}

void AGameManager::SkipTurn() {
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	EndTurn();
}

#pragma endregion


void AGameManager::kngithSetup(){
	//knight shit
}



