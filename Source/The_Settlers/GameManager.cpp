#include "GameManager.h"
#include "TimerManager.h"
#include "HexTileSpawner.h"
#include <Kismet/GameplayStatics.h>



AGameManager::AGameManager() {}
// Called when the game starts or when spawned
void AGameManager::BeginPlay() {
	UWorld* world = GetWorld();
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(world, AHexTileSpawner::StaticClass(), foundActors);
	for (AActor* foundActor : foundActors) {
		hexManager = Cast<AHexTileSpawner>(foundActor);
	}
	playerInventories.SetNumZeroed(4);
	//pushing ps
	for (int8 p = 0; p < 4; ++p) {
		FPlayerInventory player;
		player.player = (EPlayer)(p + 1);
		// for debugging
		player.bricks = 5;
		player.wheat = 5;
		player.ore = 5;
		player.wood = 5;
		player.wool = 5;
		player.settlements = 0;
		player.cities = 0;
		player.roads = 0;
		player.victoryPoints = 0;
		playerInventories[p] = player;
	}
	StartTurn();
}


void AGameManager::StartTurn() {
	thiefRound = false;
	FString message = FString::Printf(TEXT("Turn of player: %d"), (int32)CurrentPlayer);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Yellow, message);


	int dice1 = rand() % 6 + 1;
	int dice2 = rand() % 6 + 1;

	FString roll2 = FString::Printf(TEXT("Dice  rolls:    %d"), dice2 + dice1);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, roll2);

	hexManager->DiceRolled(dice1 + dice2);

	//resOut();
	GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &AGameManager::EndTurn, TurnDuration, false);

}


void AGameManager::EndTurn() {
	playerInventories[(int32)CurrentPlayer].canBuySett = true;
	// Cycle through the players
	int32 CurrentPlayerInt = (int32)CurrentPlayer;
	CurrentPlayerInt++;
	if (CurrentPlayerInt >= (int32)EPlayer::PLAYER4)
	{
		CurrentPlayerInt = 1;
	}
	CurrentPlayer = (EPlayer)CurrentPlayerInt;
	globalTurn++;

	// Start the next turn
	StartTurn();
}

void AGameManager::SkipTurn() {
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
	EndTurn();
}

void AGameManager::resOut() {
	for (int8 i = 0; i < 4; ++i) {
		FString null2 = FString::Printf(TEXT("                     "));
		GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, null2);
		FString bricks = FString::Printf(TEXT("Bricks     : %d"), playerInventories[i].bricks);
		GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Orange, bricks);
		FString wool = FString::Printf(TEXT("Wool         : %d"), playerInventories[i].wool);
		GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::White, wool);
		FString wood = FString::Printf(TEXT("Wood         : %d"), playerInventories[i].wood);
		GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Red, wood);
		FString wheat = FString::Printf(TEXT("Wheat       : %d"), playerInventories[i].wheat);
		GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Yellow, wheat);
		FString ore = FString::Printf(TEXT("Ore           : %d"), playerInventories[i].ore);
		GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Silver, ore);
		FString player = FString::Printf(TEXT("Player         : %d"), i + 1);
		GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, player);
		FString null = FString::Printf(TEXT("                     "));
		GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, null);
	}
}
void AGameManager::resOut2() {
	int debugdurr = 3.f;
	for (int8 i = 0; i < 4; ++i) {
		FString null2 = FString::Printf(TEXT("                     "));
		GEngine->AddOnScreenDebugMessage(-1, debugdurr, FColor::Purple, null2);
		FString bricks = FString::Printf(TEXT("Bricks     : %d"), playerInventories[i].bricks);
		GEngine->AddOnScreenDebugMessage(-1, debugdurr, FColor::Orange, bricks);
		FString wool = FString::Printf(TEXT("Wool         : %d"), playerInventories[i].wool);
		GEngine->AddOnScreenDebugMessage(-1, debugdurr, FColor::White, wool);
		FString wood = FString::Printf(TEXT("Wood         : %d"), playerInventories[i].wood);
		GEngine->AddOnScreenDebugMessage(-1, debugdurr, FColor::Red, wood);
		FString wheat = FString::Printf(TEXT("Wheat       : %d"), playerInventories[i].wheat);
		GEngine->AddOnScreenDebugMessage(-1, debugdurr, FColor::Yellow, wheat);
		FString ore = FString::Printf(TEXT("Ore           : %d"), playerInventories[i].ore);
		GEngine->AddOnScreenDebugMessage(-1, debugdurr, FColor::Silver, ore);
		FString player = FString::Printf(TEXT("Player         : %d"), i + 1);
		GEngine->AddOnScreenDebugMessage(-1, debugdurr, FColor::Purple, player);
		FString null = FString::Printf(TEXT("                     "));
		GEngine->AddOnScreenDebugMessage(-1, debugdurr, FColor::Purple, null);
	}
}
void FPlayerInventory::removeHalf() {
	int16 totalRes = total();
	if (totalRes < 7) {
		return;
	}
	for (int8 i = 0; i < totalRes/ 2; ++i) {
		removeOneRand();
	}
}
int16 FPlayerInventory::total() {
	FString mesage = FString::Printf(TEXT("Total INV     : %d"), this->wheat + this->bricks + this->ore + this->wool + this->wood);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, mesage);
	return this->wheat + this->bricks + this->ore + this->wool + this->wood;
}
void FPlayerInventory::removeOneRand() {
	rereoll:
	if (this->wheat == 0 && this->bricks == 0 && this->ore == 0 && this->wood == 0 && this->wool == 0) {
		return;
	}
	int randomResource = rand() % 5;
	if (randomResource == 0) {
		if (this->bricks == 0) {
			goto rereoll;
		}
		--this->bricks;
	}
	if (randomResource == 1) {
		if (this->wheat == 0) {
			goto rereoll;
		}
		--this->wheat;
	}
	if (randomResource == 2) {
		if (this->ore == 0) {
			goto rereoll;
		}
		--this->ore;
	}
	if (randomResource == 3) {
		if (this->wood == 0) {
			goto rereoll;
		}
		--this->wood;
	}
	if (randomResource == 4) {
		if (this->wool == 0) {
			goto rereoll;
		}
		--this->wool;
	}
}