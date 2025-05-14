
#include "PenaltiesKickGameMode.h"
#include "Characters/PorteroCharacter.h"
#include "Camera/CameraComponent.h"
#include "Characters/PateadorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Actors/BallActor.h"
#include "Widgets/GameStatsWidget.h"
#include "Actors/PorteriaActor.h"

APenaltiesKickGameMode::APenaltiesKickGameMode()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/Blueprints/BP_GameStatsWidget"));
    if (WidgetClassFinder.Succeeded())
    {
        GameplayWidgetClass = WidgetClassFinder.Class;
    }
}

APorteroCharacter* APenaltiesKickGameMode::SpawnPorteroCharacter()
{
    if (!PorteroCharacterClass)
    {
        UE_LOG(LogTemp, Error, TEXT("GoalkeeperCharacterClass is not assigned in PenaltiesKickGameMode"));
        return nullptr;
    }

    if (PorteroCharacterClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FVector SpawnLocation = FVector(0.f, 0.f, 300.f);
        FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);

        return GetWorld()->SpawnActor<APorteroCharacter>(PorteroCharacterClass, SpawnLocation, SpawnRotation, SpawnParams);
    }
    return nullptr;
}

APateadorCharacter* APenaltiesKickGameMode::SpawnPateadorCharacter(const FVector& ReferenceLocation)
{
    if (!PateadorCharacterClass)
    {
        UE_LOG(LogTemp, Error, TEXT("KickerCharacterClass is not assigned in PenaltiesKickGameMode"));
        return nullptr;
    }

    if (PateadorCharacterClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        // Calculamos 11 metros adelante en el eje X
        FVector SpawnLocation = ReferenceLocation + FVector(1100.f, 100.f, -200.f);
        FRotator SpawnRotation = FRotator(0.f, 180.f, 0.f);

        APateadorCharacter* Pateador = GetWorld()->SpawnActor<APateadorCharacter>(PateadorCharacterClass, SpawnLocation, SpawnRotation, SpawnParams);

        // Llamar a SpawnBall usando la ubicacion del pateador
        if (Pateador)
        {
            SpawnBall(Pateador->GetActorLocation());
        }

        return Pateador;
    }
    return nullptr;
}

ABallActor* APenaltiesKickGameMode::SpawnBall(const FVector& PateadorLocation)
{
    if (!BallClass)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnBall no esta asignado en PenaltiesKickGameMode"));
        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    FVector SpawnLocation = PateadorLocation + FVector(-300.f, -100.f, 100.f); // Ajusta si es necesario
    FRotator SpawnRotation = FRotator::ZeroRotator;

    BallReference = GetWorld()->SpawnActor<ABallActor>(BallClass, SpawnLocation, SpawnRotation, SpawnParams);

    if (BallReference)
    {
        UE_LOG(LogTemp, Warning, TEXT("Initial ball spawned correctly!"));
    }

    return BallReference;
}

void APenaltiesKickGameMode::ReubicationSpawnBall()
{
    if (!BallReference)
    {
        UE_LOG(LogTemp, Error, TEXT("BallReference does not exist. Cannot be relocated."));
        return;
    }

    APateadorCharacter* Pateador = Cast<APateadorCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), PateadorCharacterClass));
    if (!Pateador)
    {
        UE_LOG(LogTemp, Error, TEXT("Kicker not found!"));
        return;
    }

    FVector NuevaUbicacion = Pateador->GetActorLocation() + FVector(-300.f, -100.f, 100.f);
    BallReference->SetActorLocation(NuevaUbicacion);
    BallReference->SetActorRotation(FRotator::ZeroRotator);
    BallReference->ResetBall();

    UE_LOG(LogTemp, Warning, TEXT("Ball relocated to: %s"), *NuevaUbicacion.ToString());
}

APorteriaActor* APenaltiesKickGameMode::SpawnPorteria()
{
    if (!PorteriaClass)
    {
        UE_LOG(LogTemp, Error, TEXT("PorteriaClass is not assigned in PenaltiesKickGameMode!"));
        return nullptr;
    }

    if (PorteriaClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FVector SpawnLocation = FVector(-500.f, 0.f, -700.f);
        FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f);

        APorteriaActor* Porteria = GetWorld()->SpawnActor<APorteriaActor>(PorteriaClass, SpawnLocation, SpawnRotation, SpawnParams);

        return Porteria;
    }

    return nullptr;
}

void APenaltiesKickGameMode::ResetLevelComplete()
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void APenaltiesKickGameMode::RegisterShotResult(bool bWasGoal)
{
    if (bWasGoal)
    {
        GoalsScored++;
    }
    else
    {
        // UE_LOG(LogTemp, Warning, TEXT("Fallo el tiro. Total goles: %d"), GoalsScored);
    }

    CurrentShot++;
    MoralPoints += 2;
    
    if (GameplayWidget)
    {
        GameplayWidget->UpdateStats(GoalsScored, CurrentShot, MoralPoints);
    }

    if (CurrentShot >= MaxShots)
    {
        FString Message = FString::Printf(TEXT("End of the penalty round. Rebooting..."));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, this, &APenaltiesKickGameMode::ResetLevelComplete, 3.0f, false);
    }
    else
    {
        bWaitingNextShot = true;

        // Después de 2 segundos, restablecemos las botas de fuego
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(TimerHandle, this, &APenaltiesKickGameMode::ReubicationSpawnBall, 2.0f, false);

        // Esperamos para desactivar el efecto
        FTimerHandle FireBootsTimer;
        GetWorldTimerManager().SetTimer(FireBootsTimer, this, &APenaltiesKickGameMode::DeactivateFireBootsEffect, 2.0f, false);
    }
}

void APenaltiesKickGameMode::DeactivateFireBootsEffect()
{
    // Aquí es donde desactivamos el efecto de las Botas de Fuego después de 2 segundos
    if (BallReference)
    {
        BallReference->RemoveFireBootsEffect();
    }
}

void APenaltiesKickGameMode::SpendMoralPoints(int32 Amount)
{
    MoralPoints = FMath::Max(0, MoralPoints - Amount);
}

void APenaltiesKickGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (GameplayWidgetClass)
    {
        GameplayWidget = Cast<UGameStatsWidget>(CreateWidget<UUserWidget>(GetWorld(), GameplayWidgetClass));
        if (GameplayWidget)
        {
            GameplayWidget->AddToViewport();
            GameplayWidget->UpdateStats(GoalsScored, CurrentShot + 1, MoralPoints + 2);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to cast widget to UGameStatsWidget!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameplayWidgetClass is not set!"));
    }
}

int32 APenaltiesKickGameMode::GetMoralPoints() const
{
    return MoralPoints;
}
