#pragma once

#include "CoreMinimal.h"
#include "HexTile.h"
#include "GameFramework/Actor.h"
#include "GameManager.h"
#include "Thief.generated.h"

UCLASS()
class THE_SETTLERS_API AThief : public AActor{
	GENERATED_BODY()
public:	
	// Sets default values for this actor's properties
	AThief();
	// gloabl pointer to the thief to make it easier to access it from other classes
	inline static AThief* thief;
	//move thief function either by player or bot
	//takes the tile to be moved to as a parameter
	UFUNCTION(BlueprintCallable, Category = "Function")
	void moveThief(AHexTile* newTile);

private:
	//the tile the thief is currently on
	AHexTile* attachedTile;

	//the thief mesh
	UPROPERTY(EditAnywhere, Category = "Thief")
	UStaticMeshComponent* thiefMesh;
};
