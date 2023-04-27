#pragma once

#include "GameManager.h"
#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "NumberTile.h"
#include "HexTile.h"
#include "HexTileSpawner.generated.h"

UCLASS()
class THE_SETTLERS_API AHexTileSpawner : public AActor {
	GENERATED_BODY()

public:
	AHexTileSpawner();
	// the BeginPlay() method that starts then the level is loaded
	virtual void BeginPlay() override;	
	//static
	inline static AHexTileSpawner* hexManager;

	// the array of hex tiles that are used to store the hex tiles
	TArray<AHexTile*> gridArray;

	//array of all coordinates for the Catan grid
	TArray<FVector> catanGrid = {
		FVector(0.0, 0.0, 0.0),
		FVector(458.0, -793.0, 0.0),
		FVector(916.0, 0.0, 0.0),
		FVector(458.0, 793.0, 0.0),
		FVector(-458.0, 793.0, 0.0),
		FVector(-916.0, 0.0, 0.0),
		FVector(-458.0, -793.0, 0.0),
		FVector(916.0, -1586.0, 0.0),
		FVector(1374.0, -793.0, 0.0),
		FVector(1832.0, 0.0, 0.0),
		FVector(1374.0, 793.0, 0.0),
		FVector(916.0, 1586.0, 0.0),
		FVector(0.0, 1586.0, 0.0),
		FVector(-916.0, 1586.0, 0.0),
		FVector(-1374.0, 793.0, 0.0),
		FVector(-1832.0, 0.0, 0.0),
		FVector(-1374.0, -793.0, 0.0),
		FVector(-916.0, -1586.0, 0.0),
		FVector(0.0, -1586.0, 0.0)
	};

	//hex blueprints that are spawned
	UPROPERTY(EditAnywhere, Category = "Hex-Grid")
	TSubclassOf<AHexTile> FOREST;
	UPROPERTY(EditAnywhere, Category = "Hex-Grid")
	TSubclassOf<AHexTile> FIELD;
	UPROPERTY(EditAnywhere, Category = "Hex-Grid")
	TSubclassOf<AHexTile> CLAY;
	UPROPERTY(EditAnywhere, Category = "Hex-Grid")
	TSubclassOf<AHexTile> MOUNTAIN;
	UPROPERTY(EditAnywhere, Category = "Hex-Grid")
	TSubclassOf<AHexTile> PASTURE;
	UPROPERTY(EditAnywhere, Category = "Hex-Grid")
	TSubclassOf<AHexTile> DESERT;

	//the number tiles that are spawned on the hex tiles, these are blueprints that inherit from the number tile class which is an empty class that only holds the static mesh of the number tile
	UPROPERTY(EditAnywhere, Category = "Number-Grid")
	TSubclassOf<ANumberTile> TWO;
	UPROPERTY(EditAnywhere, Category = "Number-Grid")
	TSubclassOf<ANumberTile> THREE;
	UPROPERTY(EditAnywhere, Category = "Number-Grid")
	TSubclassOf<ANumberTile> FOUR;
	UPROPERTY(EditAnywhere, Category = "Number-Grid")
	TSubclassOf<ANumberTile> FIVE;
	UPROPERTY(EditAnywhere, Category = "Number-Grid")
	TSubclassOf<ANumberTile> SIX;
	UPROPERTY(EditAnywhere, Category = "Number-Grid")
	TSubclassOf<ANumberTile> EIGHT;
	UPROPERTY(EditAnywhere, Category = "Number-Grid")
	TSubclassOf<ANumberTile> NINE;
	UPROPERTY(EditAnywhere, Category = "Number-Grid")
	TSubclassOf<ANumberTile> TEN;
	UPROPERTY(EditAnywhere, Category = "Number-Grid")
	TSubclassOf<ANumberTile> ELEVEN;
	UPROPERTY(EditAnywhere, Category = "Number-Grid")
	TSubclassOf<ANumberTile> TWELVE;

	// the method responsible for earning resources
	bool DiceRolled(int32 dice);

	// used by the bots to get a random tile to move the thief to
	AHexTile* GetRandomTile();
private:
	// the bucket generator for hex tiles
	TArray<TSubclassOf<AHexTile>> hexBucket(int32 ammount);
};