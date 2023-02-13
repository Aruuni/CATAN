#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Thief.generated.h"
class  UStaticMeshComponent;
UCLASS()
class THE_SETTLERS_API AThief : public AActor{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThief();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Thief")
	UStaticMeshComponent* thiefMesh;
};
