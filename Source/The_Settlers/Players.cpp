#include "Players.h"


Players::Players(int32 Gbricks, int32 Gwool, int32 Gwood, int32 GWheat, int32 Gore){
	this->Gbricks = Gbricks;
	this->Gwool = Gwool;
	this->Gwood = Gwood;
	this->GWheat = GWheat;
	this->Gore = Gore;

	this->Gdeck = deckMaker(14, 6, 2, 2, 1);
	for (int8 i = 0; i < 4; ++i) {
		this->invs.Add(new PlayerInventory((EPlayer)(i + 1), &this->Gbricks, &this->Gwool, &this->Gwood, &this->GWheat, &this->Gore, &this->Gdeck));
	}


}




void Players::stealAll() { for (int8 i = 0; i < 4; ++i) { this->invs[i]->removeHalf(); } }
void Players::refreshAll() { for (int8 i = 0; i < 4; ++i) { this->invs[i]->reset(); } }

PlayerInventory* Players::getPlayer(EPlayer player){
	for (int i = 0; i < 4; ++i) {
		if (this->invs[i]->player == player) { return this->invs[i]; }
	}
	return nullptr;
}
void Players::resOut() {
	for (PlayerInventory* pinv : invs) {
		pinv->resOut();
	}
}
TArray<ECards> Players::deckMaker(int knight, int vp, int monopoly, int yop, int roads) {
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