#pragma once

#include "ENUMS.h"
#include "CoreMinimal.h"

class THE_SETTLERS_API PlayerInventory{
public:
	// constructor for the player inventory
	PlayerInventory(EPlayer player, int32 res);

	//enum of the player whews inventory this is
	EPlayer player;

	// the array of resources that the player has, the index of the array corresponds to the enum of the resource
	TArray<int32> Resources;

	bool buyRoad(bool free);
	bool buySettlement(bool free);
	bool upgradeSettlement();

	// releases the locks
	void reset();

	//getters for the player's pieces
	int32 getKnights();
	int32 getCardCount(ECards cardC);
	int32 getVP();
	int32 getRoads();

	// method that draws a card from the deck and adds it to the player's hand
	bool drawCard();

	// method that uses a card from the player's hand, returns true if the card was used successfully, false otherwise
	bool useCard(ECards card);

	// used for adding and removing resourced from the player's hand per type
	bool addResource(EResource resource);
	bool removeResource(EResource resource);
	
	int32 getResource(EResource resource);

	// for incrementing the number of knights
	void addKnight();

	// for adding and removing victory points
	void removeVictoryPoint();
	void addVictoryPoint();


	// removed half of the player's resources, used for the thief
	void removeHalf();

	//removed one random resource from the player's hand, used to steal from a player
	EResource removeOneRand();
private: 

	//these are the locks that are placed on a platter when they have drawn a card or building something, they are used to prevent the player from playing more than one card per turn or building more than one road per turn etc
	bool canBuySettbool = true;
	bool canBuyRoadbool = true;
	bool canUpgradebool = true;
	bool canDrawCardbool = true;
	bool canPlayCardbool = true;

	// the pieces this player owns
	int32 settlements = 0;
	int32 cities = 0;
	int32 roads = 0;
	int32 victoryPoints = 0;
	int32 knights = 0;

	// the array of cards that the player has
	TArray<ECards> hand;

	// the card that is unplayable this round as it has been drawn
	ECards unplayable;

	//for checks if a road can be bought for this player this turn
	bool canBuySett();
	bool canBuyRoad();
	bool canUpgrade();

	//returns the total number of resources the player has of all types
	int32 total();

	// returns true if the player has more than one of the card, used as a card cant be played it the player has drawn it that turn
	bool moreThanOne(ECards card);
};
