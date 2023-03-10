#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Thief.generated.h"
class AGameManager;
class  UStaticMeshComponent;
UCLASS()
class THE_SETTLERS_API AThief : public AActor{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThief();
	virtual void BeginPlay() override;


	AGameManager* game;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Thief")
	UStaticMeshComponent* thiefMesh;
	
	void removeHalf();
	void moveThief();
};
