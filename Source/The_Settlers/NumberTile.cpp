#include "NumberTile.h"
#include <Components/SceneCaptureComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>

ANumberTile::ANumberTile() {
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
    NumberMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NumberMesh"));
    NumberMesh->SetupAttachment(RootComponent);
}