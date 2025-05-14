#include "CardItemActor.h"
#include "BallActor.h"
#include "EngineUtils.h"


ACardItemActor::ACardItemActor()
{
	PrimaryActorTick.bCanEverTick = true;

    CardItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CardItemMesh"));
    RootComponent = CardItemMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CardItemMeshAsset(TEXT("/Game/01_-_Mesh/SM_PowerCardPenaltyWorld-v2_SM__PowerCardPenaltyWorld"));
    if (CardItemMeshAsset.Succeeded())
    {
        CardItemMesh->SetStaticMesh(CardItemMeshAsset.Object);
    }
}

void ACardItemActor::BeginPlay()
{
	Super::BeginPlay();
}

void ACardItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

