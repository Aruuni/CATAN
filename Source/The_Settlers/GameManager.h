#pragma once

#include "ENUMS.h"
#include "TimerManager.h"
#include "CoreMinimal.h"
#include "PlayerInventory.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

class AHexTileSpawner;

UCLASS()
class THE_SETTLERS_API AGameManager : public AActor {
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	AGameManager();
	//Global resources
	TArray<int32> gResources = {20,20,20,20,20};
	TArray<ECards> Gdeck;
	TArray<PlayerInventory*> invs;
	void refreshAll();
	void stealAll();
	void resOut();
	PlayerInventory* getPlayer(EPlayer player);
	TArray<ECards> deckMaker(int knight, int vp, int monopoly, int yop, int roads);





	//LOCKS
	//Knigth lock not necessary, just needs to change the thief lock
	inline static bool monopolyLock = false;
	inline static bool yearOPlentyLock = false;
	inline static bool thiefLock = false;

	//Turn Mechanics
	inline static EPlayer CurrentPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	ECards cardInPlay;


	// Turn duration in seconds
	//cvhange manually in the player inventories res out bc for some reason you cant have a static float???
	float TurnDuration = 5.f;
	FTimerHandle TurnTimerHandle;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn-Mechanics")
	int32 globalTurn;
	void StartTurn();
	void EndTurn();

	UFUNCTION(BlueprintCallable, Category = "Function")
	void SkipTurn();


	inline static AGameManager* gameGlobal;


	//BLUEPRINT INTEGRATION
	UFUNCTION(BlueprintCallable, Category = "Function")
	FString getResourceHUD(EPlayer player, EResource resource);
	//AUTO
	UFUNCTION(BlueprintCallable, Category = "Function")
	void monopoly_AUTO();
	UFUNCTION(BlueprintCallable, Category = "Function")
	void yearOPlenty_AUTO();
	UFUNCTION(BlueprintCallable, Category = "Function")
	void freeRoads();
	//MANUAL
	UFUNCTION(BlueprintCallable, Category = "Function")
	void monopoly(EResource resource);
	/*UFUNCTION(BlueprintCallable, Category = "Function")
	void yearOPlenty();*/



	//UFUNCTION(BlueprintCallable, Category = "Function")
	//void monolopy(EPlayer player, EResource resource);



	//UFUNCTION(BlueprintCallable, Category = "Function")
	// ? getCardNumber(EPlayer, ECard/ just return a set) to connect to the hud and turn off the cards if they are 0 
	
};