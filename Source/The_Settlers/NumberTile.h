#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexTile.h"
#include "Settlement.h"
#include "NumberTile.generated.h"

class  UstaticMeshComponent;
UENUM(BlueprintType)
enum class ENumberTile : uint8 {
	ZERO,
	TWO = 2,
	THREE = 3,
	FOUR = 4,
	FIVE = 5,
	SIX = 6,
	EIGHT = 8,
	NINE = 9,
	TEN = 10,
	ELEVEN = 11,
	TWELVE = 12
};
UCLASS()
class THE_SETTLERS_API ANumberTile : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Number-Tile")
	UStaticMeshComponent* NumberMesh;

	ANumberTile();
};