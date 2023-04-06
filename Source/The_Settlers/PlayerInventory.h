#pragma once
#include "ENUMS.h"
#include "CoreMinimal.h"

class THE_SETTLERS_API PlayerInventory{
public:
	PlayerInventory(EPlayer player, TArray<int32*> gResources, TArray<ECards>* Gdeck );
	EPlayer player;
	// pool respirces
	TArray<int32*> gResources;
	TArray<int32> Resources;
	TArray<ECards>* Gdeck;

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
	void drawCard();
	bool useCard(ECards card);

	//inv management 
	void removeHalf();
	bool addResource(EResource resource);
	int32 getResource(EResource resource);
	//void stealResource(PlayerInventory p1, PlayerInventory p2, EResource resource);
	void resOut();
private:
	bool moreThanOne(ECards card);
	int32 total();
	void removeOneRand();

};
