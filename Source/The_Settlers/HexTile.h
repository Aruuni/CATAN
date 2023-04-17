#pragma once

#include "CoreMinimal.h"
#include "Settlement.h"
#include "GameFramework/Actor.h"
#include "ENUMS.h"
#include "GameManager.h"
#include "HexTile.generated.h"

class  UstaticMeshComponent;


UCLASS()
class THE_SETTLERS_API AHexTile : public AActor{
	GENERATED_BODY()
public:
	AHexTile();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlements")
	TArray<ASettlement*> settArray;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlements")
	int32 number;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thief")
	bool hasThief = false;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex-Tile")
	EHexTile tileType;

	void settSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex-Tile")
	UStaticMeshComponent* TileMesh;

	UFUNCTION(BlueprintCallable, Category = "Function")
	EPlayer getCurrentPlayer();

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool checkThiefLock();

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool checkMySettlementAdjacency(EPlayer player);

};