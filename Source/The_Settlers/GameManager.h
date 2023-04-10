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
	inline static TArray<int32> gResources = {5,5,5,5,5};
	inline static TArray<ECards> Gdeck;


	TArray<PlayerInventory*> invs;
	void refreshAll();
	void stealAll();
	void resOut();
	PlayerInventory* getPlayer(EPlayer player);
	TArray<ECards> deckMaker(int knight, int vp, int monopoly, int yop, int roads);

	//LOCKS
	inline static bool monopolyLock = false;
	inline static bool yearOPlentyLock = false;
	inline static bool thiefLock = false;
	inline static bool freeRoadsLock = false;
	inline static int freeRoadsCount = 0;

	//Turn Mechanics
	inline static EPlayer CurrentPlayer;
	UFUNCTION(BlueprintCallable, Category = "Function")
	EPlayer getCurrentPlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	ECards cardInPlay;


	// Turn duration in seconds
	//cvhange manually in the player inventories res out bc for some reason you cant have a static float???
	float TurnDuration = 1000.f;
	FTimerHandle TurnTimerHandle;

	
	//debugging
	int32 globalTurn;

	void StartTurn();
	void EndTurn();

	UFUNCTION(BlueprintCallable, Category = "Function")
	void SkipTurn();

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool trade(EPlayer player1, EPlayer player2, EResource resource, EResource resource2);

	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 getCardCount(EPlayer player, ECards cardC);

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool useCard(EPlayer player, ECards card);

	//BLUEPRINT INTEGRATION
	UFUNCTION(BlueprintCallable, Category = "Function")
	FString getResourceHUD(EPlayer player, EResource resource);

	UFUNCTION(BlueprintCallable, Category = "Function")
	void yearOPlenty_AUTO();

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

	EPlayer largestArmyPlayer = EPlayer::NONE;

	UFUNCTION(BlueprintCallable, Category = "Function")
	void largestArmy();



	//UFUNCTION(BlueprintCallable, Category = "Function")
	//void monolopy(EPlayer player, EResource resource);

	//UFUNCTION(BlueprintCallable, Category = "Function")
	// ? getCardNumber(EPlayer, ECard/ just return a set) to connect to the hud and turn off the cards if they are 0 
	
};