#include "PlayerInventory.h"
#include "ENUMS.h"	

PlayerInventory::PlayerInventory(EPlayer player, int32* Gbricks, int32* Gwool, int32* Gwood, int32* GWheat, int32* Gore, TArray<ECards>* Gdeck) {
	this->player = player;
	this->Gbricks = Gbricks;
	this->Gwool = Gwool;
	this->Gwood = Gwood;
	this->Gwheat = GWheat;
	this->Gore = Gore;
	this->Gdeck = Gdeck;
}

#pragma region Building

#pragma region BORING GETTERS

bool PlayerInventory::canBuyRoad() { return this->canBuyRoadbool && roads <=15; }
bool PlayerInventory::canBuySett() { return this->canBuySettbool && settlements<=5; }
// do later TODO
bool PlayerInventory::canUpgrade() { return this->canUpgradebool ; }

#pragma endregion

#pragma region BuyMethods

bool PlayerInventory::buyRoad() {
	if (!canBuyRoad()) { return false; }
	if (bricks >= 1 && wood >= 1) {
		--wood;	--Gwood;
		--bricks; --Gbricks;
		++roads;
		canBuyRoadbool = false;
		return true;
	}
	else { return false; }
}

bool PlayerInventory::buySettlement() {
	if (!canBuySett()) { return false; }
	if (wheat >= 1 && bricks >= 1 && wool >= 1 && wood >= 1) {
		--wheat; --Gwheat;
		--wood;	--Gwood;
		--wool;	--Gwool;
		--bricks; --Gbricks;
		++settlements;
		++victoryPoints;
		this->canBuySettbool = false;
		return true;
	}
	else { return false; }
}

bool PlayerInventory::upgradeSettlement() {
	if (!canUpgrade()) { return false; }
	if (this->wheat >= 2 && this->ore >= 3) {
		----this->wheat;
		------this->ore;
		this->canUpgradebool = false;
		return true;
	}
	else { return false; }
}

#pragma endregion

#pragma endregion

#pragma region Cards
void PlayerInventory::drawCard() {
	if (!canDrawCardbool) { return; }
	if (this->ore >=1 && this->wheat >= 1 && this->wool >= 1) {
		--this->ore;
		--this->wheat;
		--this->wool;
		int32 randno = rand() % Gdeck->Num();
		hand.Add((*Gdeck)[randno]);
		unplayable = (*Gdeck)[randno];
		Gdeck->RemoveAt(randno);
		canDrawCardbool = false;
	}
	else { return; }
}

ECards PlayerInventory::useCard(ECards card) {
	if (card == unplayable) { if (!moreThanOne(card)) { return ECards::NONE; } }
	int32 cardIndex = hand.Find(card);
	if (cardIndex != INDEX_NONE) {
		ECards removedCard = this->hand[cardIndex];
		hand.RemoveAt(cardIndex);
		return removedCard;
	}
	return ECards::NONE;
}

bool PlayerInventory::moreThanOne(ECards card){
	int count = 0;
	for (ECards c : hand) { if (c == card) { count; } }
	return count>=2 ? true:false;
}

#pragma endregion

#pragma region Inventory Management

void PlayerInventory::reset() {
	canBuyRoadbool = true;
	canBuySettbool = true;
	canUpgradebool = true;
	canDrawCardbool = true;
	canPlayCardbool = true;
	unplayable = ECards::NONE;
}

void PlayerInventory::removeHalf() {
	int16 totalRes = total();
	if (totalRes < 8) { return; }
	for (int8 i = 0; i < totalRes / 2; ++i) { removeOneRand(); }
}



int32 PlayerInventory::total() { return bricks + ore + wool + wood + wheat; }

void PlayerInventory::removeOneRand(){
rereoll:
	if (wheat == 0 && bricks == 0 && ore == 0 && wood == 0 && wool == 0) { return; }
	int randomResource = rand() % 5;
	if (randomResource == 0) {
		if (bricks == 0) { goto rereoll; }
		--bricks; --Gbricks;
	}
	if (randomResource == 1) {
		if (wheat == 0) { goto rereoll; }
		--wheat; --Gwheat;
	}
	if (randomResource == 2) {
		if (ore == 0) { goto rereoll; }
		--ore; --Gore;
	}
	if (randomResource == 3) {
		if (wood == 0) { goto rereoll; }
		--wood; --Gwood;
	}
	if (randomResource == 4) {
		if (wool == 0) { goto rereoll; }
		--wool; --Gwool;
	}
}

void PlayerInventory::resOut(){
	float TurnDuration = 5.f;
	FString null2 = FString::Printf(TEXT("                     "));
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, null2);
	FString bricksout = FString::Printf(TEXT("Bricks     : %d"), this->bricks);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Orange, bricksout);
	FString woolout = FString::Printf(TEXT("Wool         : %d"), this->wool);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::White, woolout);
	FString woodout = FString::Printf(TEXT("Wood         : %d"), this->wood);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Red, woodout);
	FString wheatout = FString::Printf(TEXT("Wheat       : %d"), this->wheat);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Yellow, wheatout);
	FString oreout = FString::Printf(TEXT("Ore           : %d"), this->ore);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Silver, oreout);
	FString playerout = FString::Printf(TEXT("Player         : %d"), (int32)this->player);
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, playerout);
	FString null = FString::Printf(TEXT("                     "));
	GEngine->AddOnScreenDebugMessage(-1, TurnDuration, FColor::Purple, null);
	
}

void PlayerInventory::addResource(EResource resource) {
	if (resource == EResource::WHEAT) { if (*Gwheat <= 18) { ++wheat; ++Gwheat; } }
	if (resource == EResource::BRICKS) { if (*Gbricks <= 18) { ++bricks; ++Gbricks; } }
	if (resource == EResource::ORE) { if (*Gore <= 18) { ++ore; ++Gore; } }
	if (resource == EResource::WOOL) { if (*Gwool <= 18) { ++wool; ++Gwool; } }
	if (resource == EResource::WOOD) { if (*Gwood <= 18) { ++wood; ++Gwood; } }
}

void PlayerInventory::stealResource(PlayerInventory p1, PlayerInventory p2, EResource resource) {
	p1.addResource(resource);
	if (resource == EResource::WHEAT) { --p2.wheat; }
	if (resource == EResource::BRICKS) { --p2.bricks; }
	if (resource == EResource::ORE) { --p2.ore; }
	if (resource == EResource::WOOL) { --p2.wool; }
	if (resource == EResource::WOOD) { --p2.wood; }
}
#pragma endregion


