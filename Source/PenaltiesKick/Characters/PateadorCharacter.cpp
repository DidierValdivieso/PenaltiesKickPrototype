#include "PateadorCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Animation/AnimMontage.h"
#include "MainCharacter.h"
#include "../Actors/BallActor.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include <PenaltiesKick/Actors/CardItemActor.h>
#include <PenaltiesKick/PenaltiesKickGameMode.h>


APateadorCharacter::APateadorCharacter()
{
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(RootComponent);
    CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 100.0f)); // Elevar la camara
    CameraComponent->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f)); // Ajustar el angulo

    UE_LOG(LogTemp, Warning, TEXT("Kicker: Camera created and positioned"));

    static ConstructorHelpers::FObjectFinder<UAnimMontage> KickAnimationAsset(TEXT("/Game/Blueprints/AM_Pateador"));
    if (KickAnimationAsset.Succeeded())
    {
        AM_Pateador = KickAnimationAsset.Object;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("The kicking animation could not be found."));
    }

    FireFootEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireFootEffect"));
    FireFootEffect->SetupAttachment(GetMesh(), TEXT("FireFootSocket"));
    FireFootEffect->bAutoActivate = false;
    UE_LOG(LogTemp, Warning, TEXT("FireFootEffect attached to: %s"), *FireFootEffect->GetAttachSocketName().ToString());

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireEffectAsset(TEXT("/Game/FX/NS_Fire_Small"));
    if (FireEffectAsset.Succeeded())
    {
        FireFootVFX = FireEffectAsset.Object;
        FireFootEffect->SetAsset(FireFootVFX);
        
    }
}

void APateadorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    
    PlayerInputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &APateadorCharacter::OnAction);
    PlayerInputComponent->BindKey(EKeys::Z, IE_Pressed, this, &APateadorCharacter::ActivateFireBoots);
}

void APateadorCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void APateadorCharacter::OnAction()
{
    Super::OnAction();
    PlayAnimation(AM_Pateador);

    GetWorldTimerManager().SetTimer(ShootDelayHandle, this, &APateadorCharacter::PullTrigger, 0.7f, false);
}

void APateadorCharacter::PullTrigger()
{
    for (TActorIterator<ABallActor> It(GetWorld()); It; ++It)
    {
        ball = *It;
        ball->SetOwner(this);
        break;
    }

    if (!ball)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ball is nullptr"));
        return;
    }

    ball->Shoot();
}

FVector APateadorCharacter::GetPawnViewLocation() const
{
    if (CameraComponent == nullptr)
        return Super::GetPawnViewLocation();

    return CameraComponent->GetComponentLocation();
}

void APateadorCharacter::ActivateFireBoots()
{
    if (APenaltiesKickGameMode* GameMode = Cast<APenaltiesKickGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
    {
        if (GameMode->GetMoralPoints() < 3)
        {
            UE_LOG(LogTemp, Warning, TEXT("Not enough moral points."));
            return;
        }

        for (TActorIterator<ABallActor> It(GetWorld()); It; ++It)
        {
            ABallActor* Ball = *It;
            if (Ball)
            {
                Ball->ActivateFireBootsEffect();
                GameMode->SpendMoralPoints(3);
                return;
            }
        }

        UE_LOG(LogTemp, Error, TEXT("No ball found in the world."));
    }
}




