#pragma once

#include "TimerManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

class AHexTileSpawner;

USTRUCT(BlueprintType)
struct FPlayerInventory {
	GENERATED_BODY()
		EPlayer player;
	int16 wheat;
	int16 ore;
	int16 wool;
	int16 wood;
	int16 bricks;
	//max of 5
	int16 settlements;
	//max of 4
	int16 cities;
	//max of 15
	int16 roads;
	int16 victoryPoints;
	bool canBuySett = true;
	bool canBuyRoad = true;
	// maxes are conditionals on settlement buying onclick
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	bool thiefRound = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	EPlayer CurrentPlayer = EPlayer::PLAYER1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float TurnDuration = 2000000000000000000.f; // Turn duration in seconds
	FTimerHandle TurnTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	TArray<FPlayerInventory> playerInventories;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	int32 globalTurn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	AHexTileSpawner* hexManager;



	// Function to start a turn
	void StartTurn();

	// Function to end a turn
	void EndTurn();

	// Function to skip the turn
	UFUNCTION(BlueprintCallable, Category = "Function")
		void SkipTurn();


	void resOut();
	void resOut2();
};