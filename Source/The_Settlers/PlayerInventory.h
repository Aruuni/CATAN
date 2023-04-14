#pragma once

#include "ENUMS.h"
#include "CoreMinimal.h"

class THE_SETTLERS_API PlayerInventory{
public:
	PlayerInventory(EPlayer player);
	EPlayer player;
	// pool respirces
	TArray<int32> Resources;

	//this player
	int32 settlements = 0;
	int32 cities = 0;
	int32 roads = 0;
	int32 victoryPoints = 0;
	int32 knights = 0;
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
	int32 getCardCount(ECards cardC);
	bool drawCard();
	bool useCard(ECards card);
	void addKnight();
	//inv management 
	void removeHalf();
	void addVictoryPoint();
	bool addResource(EResource resource);
	bool removeResource(EResource resource);
	int32 getResource(EResource resource);
	void resOut();
private:
	bool moreThanOne(ECards card);
	int32 total();
	void removeOneRand();

};
