#include "Thief.h"
#include "GameManager.h"
#include <Kismet/GameplayStatics.h>
#include <Components/SceneCaptureComponent.h>
#include <Components/StaticMeshComponent.h>

AThief::AThief(){
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	thiefMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bMesh"));
	thiefMesh->SetupAttachment(RootComponent);
}
void AThief::BeginPlay() {
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameManager::StaticClass(), foundActors);
	for (AActor* foundActor : foundActors) {
		game = Cast<AGameManager>(foundActor);
	}

}
