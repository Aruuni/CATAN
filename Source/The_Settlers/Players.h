#pragma once

#include "PlayerInventory.h"
#include "CoreMinimal.h"
#include "ENUMS.h"


class THE_SETTLERS_API Players {
public:
	Players(int32 Gbricks, int32 Gwool, int32 Gwood, int32 GWheat, int32 Gore);
	int32 Gbricks;
	int32 Gwool;
	int32 Gwood ;
	int32 GWheat;
	int32 Gore;
	TArray<ECards> Gdeck;
	TArray<PlayerInventory*> invs;
	void refreshAll();
	void stealAll();
	void resOut();
	PlayerInventory* getPlayer(EPlayer player);
	TArray<ECards> deckMaker(int knight, int vp, int monopoly, int yop, int roads);
};
