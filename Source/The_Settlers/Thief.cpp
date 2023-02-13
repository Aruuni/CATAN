#include "Thief.h"
#include <Components/SceneCaptureComponent.h>
#include <Components/StaticMeshComponent.h>

AThief::AThief(){
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	thiefMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bMesh"));
	thiefMesh->SetupAttachment(RootComponent);
}
