# Penalties Kick Prototype

## Descripción

**PenaltiesKickPrototype** es un juego de simulación de penales desarrollado en **Unreal Engine 5** utilizando **C++**. El objetivo del juego es permitir al jugador realizar tiros de penal y enfrentar al portero en un entorno realista, con mecánicas de juego personalizadas, como el uso de cartas que alteran las propiedades del balón y el puntaje en las rondas de penal.

Este proyecto tiene como propósito explorar la implementación de mecánicas de juegos de deportes, inteligencia artificial de portero, y el uso de cartas con efectos en las físicas del balón, todo implementado directamente en C++.

## Características

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
void APateadorCharacter::UsarBotasDeFuego()
{
    BallActor->AjustarPotencia(150);
    BallActor->ReducirPrecision(15);
}
```
### 2. **Detección de Goles**
Los goles se detectan usando un **LineTrace** en el método `PerformLineTrace()` de la clase `BallActor`. Si el balón cruza la línea de gol, se contabiliza como gol.

```cpp
void ABallActor::PerformLineTrace()
{
    FHitResult HitResult;
    FVector Start = GetActorLocation();
    FVector End = Start + FVector(1000, 0, 0);
    
    // LineTrace para detectar goles
    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
    {
        if (HitResult.Actor->IsA(AGoalLine::StaticClass()))
        {
            // Gol detectado
            OnGolAnotado();
        }
    }
}
```
### 3. **Puntos de Moral y Turnos**
Cada vez que el jugador realiza un penal, se suman puntos de moral, que pueden ser consumidos para usar cartas. Los turnos también se gestionan a través del **GameMode**.

```cpp
void APenaltiesKickGameMode::GestionarTurnos()
{
    Turnos++;
    PuntosDeMoral += 2;
    
    if (Turnos > 2)
    {
        // Fin de la ronda
    }
}
```
### 4. **HUD en C++**
El HUD muestra los goles, turnos y puntos de moral utilizando la clase `UUserWidget` y una clase `GameState` que gestiona los datos.

```cpp
void AMyGameState::ActualizarHUD()
{
    HUD->ActualizarGoles(GolesAnotados);
    HUD->ActualizarTurnos(Turnos);
    HUD->ActualizarMoral(PuntosDeMoral);
}
```
## Instalación

1. Clona el repositorio:

```bash
   git clone https://github.com/DidierValdivieso/PenaltiesKickPrototype.git
```
