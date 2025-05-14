#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainController.generated.h"

class APorteroCharacter;
class APateadorCharacter;

UCLASS()
class PENALTIESKICK_API AMainController : public APlayerController
{
    GENERATED_BODY()

public:

    void BeginPlay();
    void SwitchCharacter();

    void SetPorteroRef(APorteroCharacter* Portero) { PorteroRef = Portero; }
    void SetPateadorRef(APateadorCharacter* Pateador) { PateadorRef = Pateador; }

private:
    UPROPERTY()
    APorteroCharacter* PorteroRef;

    UPROPERTY()
    APateadorCharacter* PateadorRef;

    bool bControllingPortero;

};