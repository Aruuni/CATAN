#pragma once
#include "GameManager.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement-Owner")
	EPlayer playerOwner;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement")
	AGameManager* game;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement-Level-1")
	TSubclassOf<ABuilding> Level1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement-Level-2")
	TSubclassOf<ABuilding> Level2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settlement-Level-2")
	ABuilding* building;

	UFUNCTION(BlueprintCallable, Category = "Function")
	void AddResource(EResource type);

	UFUNCTION(BlueprintCallable, Category = "Function")
	void SettlementBuyer(EPlayer player);

	UFUNCTION(BlueprintCallable, Category = "Function")
	void Upgrade();

	ASettlement();

	void stealResource(EPlayer stealer);
	void settlementLock(EPlayer player);
	bool roadAdjacency(EPlayer player);

	UFUNCTION(BlueprintCallable, Category = "Function")
	int32 checkMaxResources(EResource resource);

	virtual void BeginPlay() override;
};