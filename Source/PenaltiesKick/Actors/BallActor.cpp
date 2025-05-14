#include "BallActor.h"
#include "Kismet/GameplayStatics.h"
#include "../PenaltiesKickGameMode.h"
#include "PorteriaActor.h"
#include <PenaltiesKick/Characters/PateadorCharacter.h>

ABallActor::ABallActor()
{
    PrimaryActorTick.bCanEverTick = true;

    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    check(BallMesh);
    RootComponent = BallMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> BallMeshAsset(TEXT("/Game/Meshes/football"));
    if (BallMeshAsset.Succeeded())
    {
        BallMesh->SetStaticMesh(BallMeshAsset.Object);
    }

    FireEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FireEffect"));
    FireEffect->SetupAttachment(BallMesh);
    FireEffect->bAutoActivate = false;

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> FireAsset(TEXT("/Game/FX/NS_Fire_Small"));
    if (FireAsset.Succeeded())
    {
        FireVFX = FireAsset.Object;
        FireEffect->SetAsset(FireVFX);
    }
}

void ABallActor::BeginPlay()
{
	Super::BeginPlay();
	
    if (BallMesh)
    {
        BallMesh->SetSimulatePhysics(true);
        BallMesh->SetEnableGravity(true);
        BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        BallMesh->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
        BallMesh->SetNotifyRigidBodyCollision(true);
        BallMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    }
}

void ABallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABallActor::Shoot()
{
    if (!InitializeShootContext())
    {
        return false;
    }
    PerformLineTrace();
    FVector finalDirection = ComputeFinalShootDirection();
    ApplyImpulse(finalDirection);

    return true;
}

bool ABallActor::InitializeShootContext()
{
    OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("No owner"));
        return false;
    }

    PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController ||
        !PlayerController->GetMousePosition(MouseX, MouseY) ||
        !PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, MouseWorldOrigin, MouseWorldDirection))
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not get mouse information"));
        return false;
    }

    RayStart = GetActorLocation();
    RayEnd = MouseWorldOrigin + MouseWorldDirection * 1500.0f;

    ShootDirection = (RayEnd - RayStart).GetSafeNormal();
    RayEnd = RayStart + ShootDirection * 1500.0f;

    return true;
}

void ABallActor::PerformLineTrace()
{
    FCollisionQueryParams queryParams;
    queryParams.AddIgnoredActor(OwnerActor);
    queryParams.AddIgnoredActor(this);

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
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("There was NO collision in the raycast"));

        if (APenaltiesKickGameMode* GameMode = Cast<APenaltiesKickGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
        {
            GameMode->RegisterShotResult(false);
        }
    }
    DrawDebugLine(GetWorld(), RayStart, RayEnd, FColor::Green, true, 10.0f, 0, 1.0f);
}

FVector ABallActor::ComputeFinalShootDirection()
{
    FVector direction = ShootDirection;

    if (PrecisionOffset > 0.0f && bHitSuccess)
    {
        FVector desviatedPoint = HitResult.ImpactPoint + FVector(PrecisionOffset, 0.f, 0.f);
        direction = (desviatedPoint - RayStart).GetSafeNormal();

        FVector rayEndDesviado = RayStart + direction * 1500.0f;
        DrawDebugLine(GetWorld(), RayStart, rayEndDesviado, FColor::Red, false, 10.0f, 0, 1.5f);
        UE_LOG(LogTemp, Warning, TEXT("¡Deflected point of impact: %s!"), *desviatedPoint.ToString());
    }

    return direction;
}

void ABallActor::ApplyImpulse(const FVector& direction)
{
    float totalVelocity = Velocity_shoot + Power_shoot;
    BallMesh->SetMassOverrideInKg(NAME_None, BaseMass, true);
    BallMesh->AddImpulse(direction * totalVelocity, NAME_None, true);
    UE_LOG(LogTemp, Warning, TEXT("Velocity: %f"), totalVelocity);
}

void ABallActor::ResetBall()
{
    UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(GetRootComponent());
    if (RootComp && RootComp->IsSimulatingPhysics())
    {
        RootComp->SetPhysicsLinearVelocity(FVector::ZeroVector);
        RootComp->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

        RootComp->SetSimulatePhysics(false);
        RootComp->SetSimulatePhysics(true);

        SetActorRotation(FRotator::ZeroRotator);

        UE_LOG(LogTemp, Warning, TEXT("Ball reset!"));
    }
}

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

void ABallActor::RemoveFireBootsEffect()
{
    Power_shoot = 0.0f;
    PrecisionOffset = 0.0f;

    if (FireEffect && FireEffect->IsActive())
    {
        FireEffect->Deactivate();
    }

    UE_LOG(LogTemp, Warning, TEXT("Fire Boots effect removed."));
}



