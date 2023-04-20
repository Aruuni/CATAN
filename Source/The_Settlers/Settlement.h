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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement-Buying")
	bool locked = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement-Buying")
	bool bought = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement-Buying")
	bool upgraded = false;

	//the owner of the settlement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement-Owner")
	EPlayer playerOwner;

	//the arays of all settlements, they area all the same but have different colors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement-Level-1")
	TArray<TSubclassOf<ABuilding>> Level1;

	//the arays of all cities, they area all the same but have different colors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement-Level-2")
	TArray<TSubclassOf<ABuilding>> Level2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement-Level-2")
	ABuilding* building;

	UFUNCTION(BlueprintCallable, Category = "Function")
	void earnResource(EResource type);

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool SettlementBuyer(EPlayer player);

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool Upgrade(EPlayer player);

	ASettlement();

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool stealResource(EPlayer stealer);

	bool thiefAdjacency();

	void settlementLock();

	bool roadAdjacency(EPlayer player);

	UFUNCTION(BlueprintCallable, Category = "Function")
	EPlayer getOwner();

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool boughtChecker();
	virtual void BeginPlay() override;
};