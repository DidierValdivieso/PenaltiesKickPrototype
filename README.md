# 🎮 Penalties Kick Prototype
**PenaltiesKickPrototype** is a penalty kick simulation game developed in **Unreal Engine 5** using **C++**. The objective of the game is to allow the player to take penalty kicks and face the goalkeeper in a realistic environment, with custom gameplay mechanics, such as the use of cards that alter the ball's properties and the score in penalty rounds.

This project aims to explore the implementation of sports game mechanics, goalkeeper artificial intelligence, and the use of cards that affect ball physics, all implemented directly in C++.

---

## 🔄 Demo in action

![Gameplay Demo](TestPenaltiesKick.gif)

---

## 🛠️ Main Features

- **Penalty Kick System**: The player faces a goalkeeper and must take a penalty. The goalkeeper reacts to the shots using a basic AI system.
- **Power Cards**: A mechanic is implemented where the player can use cards that affect the ball. Example card: **Boots of Fire**, which increases shot power but decreases accuracy.
- **Penalty Rounds**: Each player has a maximum of two shots to score a goal. The round system is controlled through **GameMode** logic.
- **Morale Points**: The player obtains morale points per turn, which can be consumed to use cards that modify the ball's characteristics (power, accuracy, etc.).
- **Scoring System**: A tally is kept of goals scored, morale points, and turns, all accessible from a HUD implemented in C++.

- **Goal Logic**: Goal detection is performed using **LineTraces** to check if the ball has crossed the goal line.

## 🚀 Technologies

- **Unreal Engine 5**: Used for game development.
- **C++**: All game logic (from physics to card and AI interactions) is implemented in C++.
- **BluePrints**: The game does not use Blueprints; everything is written in C++ code to ensure greater control and performance.

## 🖥️ Project Structure

- **KickerCharacter**: Represents the player taking the kick. Includes the logic for applying cards that modify the ball's properties.
- **BallActor**: Class that handles the ball's properties, including physics, goalkeeper interaction, and goal detection.
- **CardItemActor**: Represents cards that can affect the ball, such as **Fire Boots**, which modify power and accuracy.
- **PenaltiesKickGameMode**: Controls game logic, including character creation, round management, and scoring.
- **HUD (Heads-Up Display)**: Displays goals scored, turns, and player morale points.

---

## C++ Features

### 📌 1. **Cards and Ball Properties Modification**
The card system allows you to modify the ball's properties. Example:

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
### 📌 2. **Goal Detection**
Goals are detected using a **LineTrace** in the `PerformLineTrace()` method of the `BallActor` class. If the ball crosses the goal line, it is counted as a goal.

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
### 📌 3. **Morale Points and Turns**
Each time a player takes a penalty, morale points are added, which can be used to use cards. Turns are also managed through **GameMode**.

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
### 📌 4. **HUD in C++**
The HUD displays goals, turns, and morale points using the `UUserWidget` class and a `GameState` class that manages the data.

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
---
