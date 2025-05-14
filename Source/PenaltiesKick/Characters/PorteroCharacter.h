#pragma once

#include "CoreMinimal.h"
#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "PorteroCharacter.generated.h"


UCLASS()
class PENALTIESKICK_API APorteroCharacter : public AMainCharacter
{
	GENERATED_BODY()

public:
	APorteroCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraComponent;

protected:
	virtual void OnAction() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
