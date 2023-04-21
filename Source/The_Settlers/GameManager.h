#pragma once

#include "ENUMS.h"
#include "TimerManager.h"
#include "PlayerInventory.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"


UCLASS()
class THE_SETTLERS_API AGameManager : public AActor {
	GENERATED_BODY()
public:
	//the static reference to the game manager to make it easier for other classes to access it where necessary
	inline static AGameManager* gameGlobal;
	virtual void BeginPlay() override;
	
	//constructor for the game manager
	AGameManager();
	inline static int32 resources = 0;
	//Global resources and the deck, static as they need to be accessed from anywhere and they are located only once in the translation units
	inline static TArray<int32> gResources = { resources, resources, resources, resources, resources };

	// the deck of cards that is used to draw cards from
	inline static TArray<ECards> Gdeck;

	// getter for the player inventory based on the player enum
	PlayerInventory* getPlayer(EPlayer player);
	// the deck maker function that creates the deck based on the number of cards of each type
	TArray<ECards> deckMaker(int knight, int vp, int monopoly, int yop, int roads);

	//LOCKS - inline and static so they can be accessed from anywhere and they are located only once in the translation units
	inline static bool thiefLock = false;
	inline static bool roadBuildingLock = false;
	inline static bool stealLock = false;
	// a counter needs to be kept to know how many roads the player has built with the road building card
	inline static int freeRoadsCount = 0;
	inline static EPlayer CurrentPlayer = EPlayer::PLAYER1;

	//getter for blueprint used for turn validation among other things
	UFUNCTION(BlueprintCallable, Category = "Function")
	EPlayer getCurrentPlayer();

	// maximum speed for a bot action in seconds, the higher the slower the bots act
	int32 botSpeed = 1;

	// Turn duration in seconds
	float TurnDuration = 60.f;

	//called by the thief
	void stealAll();

	//the turn number to be displayed on the HUD
	int32 globalTurn = 1;

	//the array of player inventories that are used to store the players resources and cards
	TArray<PlayerInventory*> invs;

	//Skips the turn of the current player
	UFUNCTION(BlueprintCallable, Category = "Function")
	void SkipTurn();

	//Trade function 
	//Player 1 is the player who the actual player selects
	//player 2 is the player itself
	//returns true if the trade was successful false otherwise
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool trade(EPlayer player1, EPlayer player2, EResource resource, EResource resource2);

	//draws the card from the deck in the current players hand
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool drawCard();

	//uses the card from the players hand
	//returns true if the card was used successfully false otherwise
	//this is called by the HUD
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool useCard(EPlayer player, ECards card);

	//executes the road building card behavior provided a player has the card and the card is playable 
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool freeRoads(EPlayer player);

	//executes the monopoly card behavior provided a player has the card and the card is playable 
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool monopoly(EPlayer player, EResource resource);

	//does the card behavior for the knight provided a player has the card and the card is playable 
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool knight(EPlayer player);

	//plays the development card behavior provided a player has the card and the card is playable 
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool developmentCard(EPlayer player);

	//needed in the implementation of Year of Plenty in the blueprint
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool addResource(EPlayer player, EResource resource);

	//does the maritime trading behavior from the blueprint
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool shipTrade(EPlayer player, EResource resource1, EResource resource2, int32 ammount);

	//HUD Value Getters
	// 
	//HUD function to get the player with the largest army
	UFUNCTION(BlueprintCallable, Category = "Function")
	EPlayer getLargestArmyPlayer();

	//HUD function to get the player with the longest road
	UFUNCTION(BlueprintCallable, Category = "Function")
	EPlayer getLongestRoadPlayer();

	//HUD function to get the turn number
	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getTurnNumber();

	//HUD function to get the victory points of a player
	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getVictoryPoints(EPlayer player);

	//HUD function to get the resources of a player
	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getResource(EPlayer player, EResource resource);
	
	// HUD function to get the remaining time
	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getRemainingTime();

	// HUD function to get the dice value
	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getDice();

	// HUD function called in the blueprint get value binding for all players to get the number cards
	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getCardCount(EPlayer player, ECards cardC);

	// HUD function called in the blueprint get value binding for all players to get the number of knights they have
	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 knightCount(EPlayer player);


	// called on tick to end the game
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool END_GAME();

private:

	//the dice value total
	int32 dice;
	// players with the largest army and longest road
	EPlayer largestArmyPlayer = EPlayer::NONE;
	EPlayer longestRoadPlayer = EPlayer::NONE;

	//timer handles for the turn and bot
	FTimerHandle TurnTimerHandle;
	FTimerHandle botTimeHandle;

	// the functions called by the bot action loop
	bool buyRandomRoad(EPlayer player);
	bool buyRandomSett(EPlayer player);
	bool upgradeRandomSettlements(EPlayer player);

	//the bot action loop
	void startBot();
	void endBot();
	bool botAction();

	// start and end turn functions, these never get called by blueprints 
	void StartTurn();
	void EndTurn();

	//this function is called to release the locks on the player inventories such as card drawn, card played, etc
	void refreshAll();

	//called by the timer to end the turn
	void largestArmy();
	void longestRoad();

	//the year of plenty card behavior of the bots
	bool yearOPlenty(EPlayer player);

	//this is a function that is used to shuffle the arrays of settlements and roads.
	void ShuffleTArray(TArray<AActor*>& MyArray);
};