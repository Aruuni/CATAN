#include "GameManager.h"
#include "TimerManager.h"
#include "HexTileSpawner.h"
#include <Kismet/GameplayStatics.h>


AGameManager::AGameManager() {}
// Called when the game starts or when spawned
void AGameManager::BeginPlay() {
	for (int8 i = 0; i < 26; ++i) {
		if (i < 15) {
			globalDeck.Add(ECards::KNIGHT);
			continue;
		}
		if (i < 20) {
			globalDeck.Add(ECards::VICTORYPOINT);
			continue;
		}
		if (i < 22) {
			globalDeck.Add(ECards::MONOPOLY);
			continue;
		}
		if (i < 24) {
			globalDeck.Add(ECards::YEAROFPLENTY);
			continue;
		}
		if (i < 26) {
			globalDeck.Add(ECards::FREEROAD);
			continue;
		}
	}
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHexTileSpawner::StaticClass(), foundActors);
	for (AActor* foundActor : foundActors) {
		hexManager = Cast<AHexTileSpawner>(foundActor);
	}
	for (int8 p = 0; p < 4; ++p) {
		FPlayerInventory player;
		player.game = this;
		player.player = (EPlayer)(p + 1);
		player.bricks = 5;
		player.wheat = 5;
		player.ore = 5;
		player.wood = 5;
		player.wool = 5;
		player.settlements = 0;
		player.cities = 0;
		player.roads = 0;
		player.victoryPoints = 0;
		playerInventories.Add(player);
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

	resOut();
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
	playerInventories[(int32)CurrentPlayer].resetCards();
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
void AGameManager::drawCard(EPlayer drawer) {
	this->playerInventories[(int32)drawer-1].drawCard();
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
void AGameManager::playCard(EPlayer player, ECards card) {
	cardInPlay = playerInventories[(int32)player+1].playCard(card);
	if (cardInPlay == ECards::KNIGHT) {

	}
}
void AGameManager::kngithSetup(){
	//knight shit
}


void FPlayerInventory::removeHalf() {
	int16 totalRes = total();
	if (totalRes < 8) {
		return;
	}
	for (int8 i = 0; i < totalRes/ 2; ++i) {
		removeOneRand();
	}
}
int16 FPlayerInventory::total() {
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
void FPlayerInventory::drawCard() {
	FString message = FString::Printf(TEXT("card added"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, message);
	if (game->globalDeck.Num() == 0) {
		return;
	}
	if (this->ore ==0 || this->wheat ==	0 || this->wool == 0) {
		return;
	}

	--this->ore;
	--this->wheat;
	--this->wool;
	
	int32 randomCard = rand() % game->globalDeck.Num();
	this->hand.Add(game->globalDeck[randomCard]);
	unplayable = game->globalDeck[randomCard];
	game->globalDeck.RemoveAt(randomCard);
	this->drawed = true;
}
void FPlayerInventory::resetCards() {
	this->drawed = false;
	this->cardplayed = false;
	this->unplayable = ECards::NONE;
}
ECards FPlayerInventory::playCard(ECards card) {
	if (this->player == game->CurrentPlayer) {
		if (canPlayCard(card)) {
			hand.Remove(card);
			return card;
		}
	}
	return ECards::NONE;
}
bool FPlayerInventory::canPlayCard(ECards card) {
	if (hand.Contains(card)) {
		if (card == unplayable) {
			int count = 0;
			for (int8 i = 0; i < hand.Num(); ++i) {
				if (hand[i] == card) {
					++count;
				}
			}
			if (count > 1) {
				return true;
			}
		}
		return true;
	}
	return false;
}