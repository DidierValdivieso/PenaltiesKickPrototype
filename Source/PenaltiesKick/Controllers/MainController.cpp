#include "MainController.h"
#include "../PenaltiesKickGameMode.h"
#include "../Characters/PorteroCharacter.h"
#include "../Characters/PateadorCharacter.h"
#include "Kismet/GameplayStatics.h"

void AMainController::BeginPlay()
{
    Super::BeginPlay();

    if (!GetPawn())
    {
        APenaltiesKickGameMode* GM = Cast<APenaltiesKickGameMode>(UGameplayStatics::GetGameMode(this));
        if (GM)
        {
            APorteroCharacter* Portero = GM->SpawnPorteroCharacter();
            if (Portero)
            {
                Possess(Portero);
                SetPorteroRef(Portero);
                bControllingPortero = true;

                APateadorCharacter* Pateador = GM->SpawnPateadorCharacter(Portero->GetActorLocation());
                if (Pateador)
                {
                    SetPateadorRef(Pateador);
                }
            }
        }
    }

    InputComponent = NewObject<UInputComponent>(this);
    InputComponent->RegisterComponent();
    InputComponent->BindKey(EKeys::F, IE_Pressed, this, &AMainController::SwitchCharacter);
    PushInputComponent(InputComponent);
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
}

void AMainController::SwitchCharacter()
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green,
            FString::Printf(TEXT("Changing character. Current: %s"),
                bControllingPortero ? TEXT("Kicker") : TEXT("Goalkeeper")));
    }

    if (bControllingPortero && PateadorRef)
    {
        Possess(PateadorRef);
        bControllingPortero = false;
    }
    else if (!bControllingPortero && PorteroRef)
    {
        Possess(PorteroRef);
        bControllingPortero = true;
    }
}