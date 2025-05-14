# Penalties Kick Prototype
**PenaltiesKickPrototype** es un juego de simulación de penales desarrollado en **Unreal Engine 5** utilizando **C++**. El objetivo del juego es permitir al jugador realizar tiros de penal y enfrentar al portero en un entorno realista, con mecánicas de juego personalizadas, como el uso de cartas que alteran las propiedades del balón y el puntaje en las rondas de penal.

Este proyecto tiene como propósito explorar la implementación de mecánicas de juegos de deportes, inteligencia artificial de portero, y el uso de cartas con efectos en las físicas del balón, todo implementado directamente en C++.

---

## 🔄 Demo en acción

![Gameplay Demo](TestPenaltiesKick.gif)

---

## 🛠️ Características principales

- **Sistema de Penaltis**: El jugador se enfrenta a un portero y tiene que lanzar un penal. El portero reacciona a los tiros utilizando un sistema básico de IA.
- **Cartas de Poder**: Se implementa una mecánica en la que el jugador puede usar cartas que afectan al balón. Ejemplo de carta: **Botas de Fuego**, que aumenta la potencia del tiro pero disminuye la precisión.
- **Rondas de Penaltis**: Cada jugador tiene un máximo de dos disparos para marcar un gol. El sistema de rondas se controla a través de la lógica del **GameMode**.
- **Puntos de Moral**: El jugador obtiene puntos de moral por turno, que pueden ser consumidos para usar cartas que modifican las características del balón (potencia, precisión, etc.).
- **Sistema de Puntuación**: Se lleva un conteo de los goles anotados, los puntos de moral, y los turnos, todo accesible desde un HUD implementado en C++.
- **Lógica de Goles**: La detección de goles se realiza mediante **LineTraces** para comprobar si el balón ha cruzado la línea de gol.
  
## Tecnologías

- **Unreal Engine 5**: Utilizado para el desarrollo del juego.
- **C++**: Toda la lógica del juego (desde la física hasta la interacción de las cartas y la IA) está implementada en C++.
- **BluePrints**: El juego no utiliza Blueprints; todo está hecho desde código C++ para garantizar un mayor control y rendimiento.

## Estructura del Proyecto

- **PateadorCharacter**: Representa al jugador que realiza el tiro. Incluye la lógica para aplicar cartas que modifican las propiedades del balón.
- **BallActor**: Clase que maneja las propiedades del balón, incluidas las físicas, la interacción con el portero, y la detección de goles.
- **CardItemActor**: Representa las cartas que pueden afectar al balón, como las **Botas de Fuego**, que modifican la potencia y precisión.
- **PenaltiesKickGameMode**: Controla la lógica del juego, incluyendo la creación de personajes, la gestión de rondas y el puntaje.
- **HUD (Heads-Up Display)**: Muestra en pantalla los goles anotados, los turnos y los puntos de moral del jugador.

## Funcionalidades en C++

### 1. **Cartas y Modificación de Propiedades del Balón**
El sistema de cartas permite modificar las propiedades del balón. Ejemplo:

```cpp
void ABallActor::ActivateFireBootsEffect()
{
    Power_shoot += 150.0f;
    PrecisionOffset = 15.0f + Power_shoot;

    if (FireEffect && !FireEffect->IsActive())
    {
        FireEffect->Activate(true);
    }

    UE_LOG(LogTemp, Warning, TEXT("Fire Boots activated: Power = %f, PrecisionOffset = %f"), Power_shoot, PrecisionOffset);
}
```
### 2. **Detección de Goles**
Los goles se detectan usando un **LineTrace** en el método `PerformLineTrace()` de la clase `BallActor`. Si el balón cruza la línea de gol, se contabiliza como gol.

```cpp
bHitSuccess = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        RayStart,
        RayEnd,
        ECC_GameTraceChannel1,
        queryParams);

    if (bHitSuccess)
    {
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 15.f, 12, FColor::Yellow, false, 10.0f);
        UE_LOG(LogTemp, Warning, TEXT("¡Original impact on: %s!"), *HitResult.ImpactPoint.ToString());

        bool bFueGol = HitResult.GetActor() && HitResult.GetActor()->IsA(APorteriaActor::StaticClass());

        if (APenaltiesKickGameMode* GameMode = Cast<APenaltiesKickGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
        {
            GameMode->RegisterShotResult(bFueGol);
        }
    }
```
### 3. **Puntos de Moral y Turnos**
Cada vez que el jugador realiza un penal, se suman puntos de moral, que pueden ser consumidos para usar cartas. Los turnos también se gestionan a través del **GameMode**.

```cpp
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
```
### 4. **HUD en C++**
El HUD muestra los goles, turnos y puntos de moral utilizando la clase `UUserWidget` y una clase `GameState` que gestiona los datos.

```cpp
#include "GameStatsWidget.h"
#include "Components/TextBlock.h"

void UGameStatsWidget::UpdateStats(int32 Goals, int32 Turn, int32 Moral)
{
    if (GoalsText)
        GoalsText->SetText(FText::Format(FText::FromString("Goals: {0}"), FText::AsNumber(Goals)));

    if (TurnText)
        TurnText->SetText(FText::Format(FText::FromString("Turn: {0}"), FText::AsNumber(Turn)));

    if (MoralText)
        MoralText->SetText(FText::Format(FText::FromString("Moral: {0}"), FText::AsNumber(Moral)));
}
```
