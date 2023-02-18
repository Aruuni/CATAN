#pragma once

#include "TimerManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

class ACardManager;
class AHexTileSpawner;
UENUM(BlueprintType)
enum class ECards :uint8 {
	NONE,
	VICTORYPOINT,
	KNIGHT,
	FREEROAD,
	YEAROFPLENTY,
	MONOPOLY
};

USTRUCT(BlueprintType)
struct FPlayerInventory {
	GENERATED_BODY()
	
	AGameManager* game;
	EPlayer player;
	int16 wheat;
	int16 ore;
	int16 wool;
	int16 wood;
	int16 bricks;
	int16 settlements;
	int16 cities;
	int16 roads;
	int16 victoryPoints;

	bool canBuySett = true;
	bool canBuyRoad = true;


	bool drawed = false;
	bool cardplayed = false;
	ECards unplayable;
	
	TArray<ECards> hand;
	//resources
	int16 total();	
	void removeOneRand();
	void removeHalf();


	void drawCard();
	ECards playCard(ECards card);
	void resetCards();
	bool canPlayCard(ECards card);
};
UENUM(BlueprintType)
enum class EResource : uint8 {
	ZERO,
	BRICKS = 1,
	ORE = 2,
	WOOD = 3,
	WOOL = 4,
	WHEAT = 5
};

//enum of player, NONE is default
UENUM(BlueprintType)
enum class EPlayer : uint8 {
	NONE,
	PLAYER1 = 1,
	PLAYER2 = 2,
	PLAYER3 = 3,
	PLAYER4 = 4
};

UCLASS()
class THE_SETTLERS_API AGameManager : public AActor {
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	AGameManager();
	//Turn Mechanics
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn-Mechanics")
	EPlayer CurrentPlayer = EPlayer::PLAYER1;

	TArray<ECards> globalDeck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn-Mechanics")
	float TurnDuration = 2.f; // Turn duration in seconds
	FTimerHandle TurnTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn-Mechanics")
	int32 globalTurn;
	void StartTurn();
	void EndTurn();
	UFUNCTION(BlueprintCallable, Category = "Function")
	void SkipTurn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool thiefRound = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	ECards cardInPlay;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	TArray<FPlayerInventory> playerInventories;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	AHexTileSpawner* hexManager;
	UFUNCTION(BlueprintCallable, Category = "Function")


	void resOut();

	void resOut2();
	// cards 
	void drawCard(EPlayer drawer);
	void playCard(EPlayer player, ECards card);

	void kngithSetup();
	void monopoly();
};