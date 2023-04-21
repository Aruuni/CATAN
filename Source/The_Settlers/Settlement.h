#pragma once

#include "GameManager.h"
#include "ENUMS.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Road.h"
#include "Building.h"
#include "Settlement.generated.h"

class  UstaticMeshComponent;

UCLASS()
class THE_SETTLERS_API ASettlement : public AActor
{
	GENERATED_BODY()
public:
	ASettlement();
	// properties that are used to determine the state of the settlement
	bool locked = false;
	bool bought = false;
	bool upgraded = false;

	//the owner of the settlement
	EPlayer playerOwner;

	// both of the level1 and level2 buildings need to be public as they are assigned in the class settings of the settlement blueprint
	//the arrays of all settlements, they area all the same but have different colors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement-Level-1")
	TArray<TSubclassOf<ABuilding>> Level1;

	//the arrays of all cities, they area all the same but have different colors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement-Level-2")
	TArray<TSubclassOf<ABuilding>> Level2;

	// used to increment the resource of the owner of the settlement
	void earnResource(EResource type);

	// used to buy the settlement, called from blueprint as well as the bots so it is a ufunction
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool SettlementBuyer(EPlayer player);

	// used to upgrade the settlement, called from blueprint as well as the bots so it is a ufunction
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool Upgrade(EPlayer player);

	// steals a resource from the owner of the settlement
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool stealResource(EPlayer stealer);

	// used in some blueprint for validation purposes (displaying a correct error message)
	UFUNCTION(BlueprintCallable, Category = "Function")
	EPlayer getOwner();

	// used in some blueprint for validation purposes (displaying a correct error message)
	UFUNCTION(BlueprintCallable, Category = "Function")
	bool boughtChecker();

private:
	// checks thief adjacency in order to determine if the settlement can be stolen from
	bool thiefAdjacency();

	// called when the settlement is bout, as no adjacent settlements can be bought
	void settlementLock();

	// the building that is currently on the settlement
	ABuilding* building;

	// used to determine if the settlement is adjacent to a road when buying it
	bool roadAdjacency(EPlayer player);

};