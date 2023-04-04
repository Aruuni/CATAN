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

	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thief")
	AHexTile* attchedTile;

	inline static AThief* thief;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Thief")
	UStaticMeshComponent* thiefMesh;

	UFUNCTION(BlueprintCallable, Category = "Function")
	void moveThief(FVector loc, AHexTile* newLocation);

};
