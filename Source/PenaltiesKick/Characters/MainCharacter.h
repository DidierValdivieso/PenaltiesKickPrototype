#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS(Abstract) // ¡Muy importante! Para que no puedas instanciar directamente este personaje.
class PENALTIESKICK_API AMainCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMainCharacter();

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void PlayAnimation(UAnimMontage* Animation);
    virtual void OnAction();

    UFUNCTION()
    void SwitchCharacter();

    UFUNCTION()
    void HandleRestartRequest();

protected:
    virtual void BeginPlay() override;

};