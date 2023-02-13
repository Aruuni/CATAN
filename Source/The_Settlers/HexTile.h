#pragma once

#include "CoreMinimal.h"
#include "Settlement.h"
#include "GameFramework/Actor.h"
#include "HexTile.generated.h"
class AThief;
class  UstaticMeshComponent;

UENUM(BlueprintType)
enum class EHexTile : uint8 {
	DESERT,
	CLAY = 1,
	MOUNTAIN = 2,
	FORREST = 3,
	PASTURE = 4,
	FIELD = 5,
	SEA = 6,
	TRADE = 7
};
UCLASS()
class THE_SETTLERS_API AHexTile : public AActor
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlements")
	TArray<ASettlement*> settArray;
	UPROPERTY(EditAnywhere, Category = "Thief")
	TSubclassOf<AThief> thiefMesh;

	AHexTileSpawner* hexSpawner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlements")
	int32 number;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thief")
	bool hasThief = false;

	AHexTile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex-Tile")
	EHexTile tileType;

	void settSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex-Tile")
	UStaticMeshComponent* TileMesh;

	UFUNCTION(BlueprintCallable, Category = "Function")
	void thiefMove();
};