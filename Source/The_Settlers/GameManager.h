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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn-Mechanics")
	EPlayer CurrentPlayer = EPlayer::PLAYER1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	ECards cardInPlay;

	Players* players;

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
	AHexTileSpawner* hexManager;
	UFUNCTION(BlueprintCallable, Category = "Function")

	void kngithSetup();
	void monopoly();
	// shoudl be private
	TArray<ECards> deckMaker(int knight, int vp, int monopoly, int yop, int roads);
};