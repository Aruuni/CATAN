#pragma once

#include "CoreMinimal.h"
#include "Settlement.h"
#include "GameFramework/Actor.h"
#include "ENUMS.h"
#include "HexTile.generated.h"

class  UstaticMeshComponent;


UCLASS()
class THE_SETTLERS_API AHexTile : public AActor{
	GENERATED_BODY()
public:
	AHexTile();
	//settlement array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlements")
	TArray<ASettlement*> settArray;

	//the hextile number
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlements")
	int32 number;

	//the hasThief value prevents the earning of resources
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thief")
	bool hasThief = false;

	//the hextile type, compatible with the resource enum
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex-Tile")
	EHexTile tileType;

	//the array of adjacent settlements-
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool checkMySettlementAdjacency(EPlayer player); 

	//the setter for the adjacent selttlement arrays
	void settSet();

private:
	//the mesh of the hexTile
	UPROPERTY(EditAnywhere, Category = "Hex-Tile")
	UStaticMeshComponent* TileMesh;

};