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
	inline static AGameManager* gameGlobal;
	virtual void BeginPlay() override;
	AGameManager();

	//Global resources
	inline static TArray<int32> gResources = {10,10,10,10,10};
	inline static TArray<ECards> Gdeck;


	TArray<PlayerInventory*> invs;
	void refreshAll();
	void stealAll();	
	PlayerInventory* getPlayer(EPlayer player);
	TArray<ECards> deckMaker(int knight, int vp, int monopoly, int yop, int roads);

	//LOCKS
	inline static bool monopolyLock = false;
	inline static bool yearOPlentyLock = false;
	inline static bool thiefLock = false;
	inline static bool freeRoadsLock = false;
	inline static int freeRoadsCount = 0;
	inline static EPlayer CurrentPlayer;
	EPlayer largestArmyPlayer = EPlayer::NONE;
	EPlayer longestRoadPlayer = EPlayer::NONE;

	
	UFUNCTION(BlueprintCallable, Category = "Function")
	EPlayer getCurrentPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	ECards cardInPlay;


	// Turn duration in seconds
	//cvhange manually in the player inventories res out bc for some reason you cant have a static float???
	float TurnDuration = 1000.f;

	FTimerHandle TurnTimerHandle;

	//debugging
	int32 globalTurn = 1;
	int32 dice;
	void StartTurn();
	void EndTurn();



	UFUNCTION(BlueprintCallable, Category = "Function")
	void SkipTurn();

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool trade(EPlayer player1, EPlayer player2, EResource resource, EResource resource2);



	UFUNCTION(BlueprintCallable, Category = "Function")
	bool useCard(EPlayer player, ECards card);

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool drawCard();



	void yearOPlenty();
	//Card functionality functions coupled to the HUD
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool freeRoads(EPlayer player);

	UFUNCTION(BlueprintCallable, Category = "Function")
	void monopoly(EResource resource);

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool knight(EPlayer player);

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool victoryPoint(EPlayer player);

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool addResource(EPlayer player, EResource resource);

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool shipTrade(EPlayer player, EResource resource1, EResource resource2);
	void largestArmy();
	
	//HUD Value Getters
	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getLargestArmyPlayer();

	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getLongestRoadPlayer();

	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getPlayerTurn();

	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getTurnNumber();

	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getVictoryPoints(EPlayer player);

	UFUNCTION(BlueprintCallable, Category = "Function")
	FString getResourceHUD(EPlayer player, EResource resource);
	
	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getRemainingTime();

	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getDice();

	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getCardCount(EPlayer player, ECards cardC);

	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 knightCount(EPlayer player);
};