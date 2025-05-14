#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "../Controllers/MainController.h"
#include <PenaltiesKick/PenaltiesKickGameMode.h>
#include <Kismet/GameplayStatics.h>

AMainCharacter::AMainCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMainCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void AMainCharacter::PlayAnimation(UAnimMontage* Animation)
{
    if (Animation)
    {
        PlayAnimMontage(Animation);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("The montage is NULL"));
    }
}

void AMainCharacter::OnAction()
{
    
}

void AMainCharacter::SwitchCharacter()
{
    AController* MyController = GetController();
    if (MyController)
    {
        AMainController* MainController = Cast<AMainController>(MyController);
        if (MainController)
        {
            MainController->SwitchCharacter();
        }
    }
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);

    PlayerInputComponent->BindKey(EKeys::R, IE_Pressed, this, &AMainCharacter::HandleRestartRequest);
}

void AMainCharacter::HandleRestartRequest()
{
    if (APenaltiesKickGameMode* GM = Cast<APenaltiesKickGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        GM->ResetLevelComplete();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Could not cast GameMode"));
    }
}
