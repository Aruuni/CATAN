#pragma once
#include "GameManager.h"
#include "Building.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Road.generated.h"

UCLASS()
class THE_SETTLERS_API ARoad : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROAD")
	bool bought = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROAD")
	EPlayer playerOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ROAD")
	TArray<TSubclassOf<ABuilding>> roads;
	ARoad();


	UFUNCTION(BlueprintCallable, Category = "Function")
	bool RoadBuyer(EPlayer player);

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool RoadDetector(EPlayer player);

	UFUNCTION(BlueprintCallable, Category = "Function")
	bool SettlementDetector(EPlayer player);
};
