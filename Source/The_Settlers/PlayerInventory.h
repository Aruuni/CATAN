#pragma once
#include "ENUMS.h"
#include "CoreMinimal.h"

class THE_SETTLERS_API PlayerInventory{
public:
	PlayerInventory(EPlayer player, int32* Gbricks,  int32* Gwool, int32* Gwood, int32* GWheat, int32* Gore, TArray<ECards>* Gdeck );
	EPlayer player;
	// pool respirces
	int32* Gbricks;
	int32* Gwool;
	int32* Gwood;
	int32* Gwheat;
	int32* Gore;
	TArray<ECards>* Gdeck;

	//this player
	int32 wheat = 5;
	int32 ore = 5;
	int32 wool = 5;
	int32 wood = 5;
	int32 bricks = 5;
	int32 settlements = 0;
	int32 cities = 0;
	int32 roads = 0;
	int32 victoryPoints = 0;
	TArray<ECards> hand;

	ECards unplayable;
	bool canBuySettbool = true;
	bool canBuyRoadbool = true;
	bool canUpgradebool = true;
	bool canDrawCardbool = true;
	bool canPlayCardbool = true;

	//buying stuff
	//wool wood wheat brick 
	bool canBuySett();
	bool canBuyRoad();
	bool canUpgrade();

	bool buyRoad();
	bool buySettlement();
	bool upgradeSettlement();
	void reset();
	//cards management 
	void drawCard();
	ECards useCard(ECards card);

	//inv management 
	void removeHalf();
	void addResource(EResource resource);
	int32 getResource(EResource resource);
	void stealResource(PlayerInventory p1, PlayerInventory p2, EResource resource);
	//private
	bool moreThanOne(ECards card);
	int32 total();
	void removeOneRand();
	void resOut();
};
