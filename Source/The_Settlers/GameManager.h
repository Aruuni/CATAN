#pragma once

#include "ENUMS.h"
#include "TimerManager.h"
#include "CoreMinimal.h"
#include "PlayerInventory.h"
#include "Players.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

class AHexTileSpawner;

UCLASS()
class THE_SETTLERS_API AGameManager : public AActor {
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	AGameManager();
	//Turn Mechanics
	inline static EPlayer CurrentPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	ECards cardInPlay;

	inline static Players* players;

	inline static AGameManager* gameGlobal;


	// Turn duration in seconds
	//cvhange manually in the player inventories res out bc for some reason you cant have a static float???
	float TurnDuration = 5.f;
	FTimerHandle TurnTimerHandle;

	inline static bool thiefLock = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn-Mechanics")
	int32 globalTurn;
	void StartTurn();
	void EndTurn();

	UFUNCTION(BlueprintCallable, Category = "Function")
	void SkipTurn();




	UFUNCTION(BlueprintCallable, Category = "Function")

	void kngithSetup();
	void monopoly();
	// shoudl be private
	TArray<ECards> deckMaker(int knight, int vp, int monopoly, int yop, int roads);
};