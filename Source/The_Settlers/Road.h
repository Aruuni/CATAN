#pragma once

#include "Building.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Road.generated.h"

UCLASS()
class THE_SETTLERS_API ARoad : public AActor {
	GENERATED_BODY()
public:
	ARoad();

	// if the road was bought or not
	bool bought = false;

	//the owner of the road
	EPlayer playerOwner;
	
	//the array of building blueprints of the roads, they are all the same but have different colors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROAD")
	TArray<TSubclassOf<ABuilding>> roads;
	

	//called by the blueprint or the bots to buy the road
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool RoadBuyer(EPlayer player);
private:
	// used to check road adjacency in order to determine if the road can be bought
	bool roadAdjacency(EPlayer player);
	
	// checking settlement adjacency, only useful in the first 8 turns
	bool settlementAdjacency(EPlayer player);

	// checking enemy settlement adjacency
	bool enemySettlementAdjacency(EPlayer player);
};
