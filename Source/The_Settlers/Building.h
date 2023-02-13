#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"
class  UStaticMeshComponent;

UCLASS()
class THE_SETTLERS_API ABuilding : public AActor{
	GENERATED_BODY()

public:

	ABuilding();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Hex-Tile")
	UStaticMeshComponent* buildingMesh;
};