#include "PorteroCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

APorteroCharacter::APorteroCharacter()
{
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    CameraComponent->SetupAttachment(RootComponent);
    CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 100.0f));
    CameraComponent->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

    UE_LOG(LogTemp, Warning, TEXT("Goalkeeper: Camera created and positioned"));
}

void APorteroCharacter::OnAction()
{
    UE_LOG(LogTemp, Warning, TEXT("¡The goalkeeper clicked to save!"));
}

void APorteroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    // Detectar clic izquierdo para ejecutar acción
    PlayerInputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &APorteroCharacter::OnAction);
}