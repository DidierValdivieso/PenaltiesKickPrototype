#include "PorteriaActor.h"

APorteriaActor::APorteriaActor()
{
	PrimaryActorTick.bCanEverTick = true;

    PorteriaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PorteriaMesh"));
    RootComponent = PorteriaMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PorteriaMeshAsset(TEXT("/Game/Meshes/soccerGoal"));
    if (PorteriaMeshAsset.Succeeded())
    {
        PorteriaMesh->SetStaticMesh(PorteriaMeshAsset.Object);
    }
}

void APorteriaActor::BeginPlay()
{
	Super::BeginPlay();	
}

void APorteriaActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

