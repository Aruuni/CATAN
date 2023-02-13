#pragma once

#include "GameManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NumberTile.h"
#include "HexTileSpawner.generated.h"

class AHexTile;
UCLASS()
class THE_SETTLERS_API AHexTileSpawner : public AActor {
	GENERATED_BODY()

public:
	AHexTileSpawner();
	virtual void BeginPlay() override;
	AGameManager* game;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex-Grid")
	TArray<AHexTile*> gridArray;
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
	//hex grid
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
	//number grid
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
	//thief
	UPROPERTY(EditAnywhere, Category = "Thief")
	TSubclassOf<AThief> thiefMesh;

	bool DiceRolled(int32 dice);

	void thiefInnit();

	void resetThief();
};