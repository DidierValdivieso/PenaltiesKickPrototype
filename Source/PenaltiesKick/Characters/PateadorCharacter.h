#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "../Actors/BallActor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "PateadorCharacter.generated.h"

UCLASS()
class PENALTIESKICK_API APateadorCharacter : public AMainCharacter
{
    GENERATED_BODY()

public:
    APateadorCharacter();
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    void BeginPlay();
    FTimerHandle ShootDelayHandle;
    FVector GetPawnViewLocation() const override;
    void ActivateFireBoots();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
    UCameraComponent* CameraComponent;    

    UPROPERTY(VisibleAnywhere)
    UNiagaraComponent* FireFootEffect;

    UPROPERTY(EditAnywhere)
    UNiagaraSystem* FireFootVFX;

protected:
    virtual void OnAction() override;

private:
    UFUNCTION()
    void PullTrigger();

    UPROPERTY()
    ABallActor* ball;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    UAnimMontage* AM_Pateador;
};