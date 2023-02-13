#include "Building.h"
#include <Components/SceneCaptureComponent.h>
#include <Components/StaticMeshComponent.h>

ABuilding::ABuilding(){
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
    buildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bMesh"));
    buildingMesh->SetupAttachment(RootComponent);
}

