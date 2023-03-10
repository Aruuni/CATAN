#pragma once

#include "CoreMinimal.h"
#include "Settlement.h"
#include "GameFramework/Actor.h"
#include "ENUMS.h"
#include "HexTile.generated.h"
class AThief;
class  UstaticMeshComponent;


UCLASS()
class THE_SETTLERS_API AHexTile : public AActor{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlements")
	TArray<ASettlement*> settArray;

	UPROPERTY(EditAnywhere, Category = "Thief")
	TSubclassOf<AThief> thiefMesh;

	AGameManager* game;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlements")
	int32 number;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thief")
	bool hasThief = false;
	AThief* thief;
	AHexTile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex-Tile")
	EHexTile tileType;

	void settSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex-Tile")
	UStaticMeshComponent* TileMesh;

	UFUNCTION(BlueprintCallable, Category = "Function")
	void thiefMove(EPlayer stealer);
};