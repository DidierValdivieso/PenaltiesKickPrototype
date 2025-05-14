#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "BallActor.generated.h"

UCLASS()
class PENALTIESKICK_API ABallActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABallActor();
	virtual void Tick(float DeltaTime) override;
	bool Shoot();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BallMesh;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UNiagaraSystem* FireVFX;

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	UNiagaraComponent* FireEffect;

	UFUNCTION()
	void ResetBall();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
	float Velocity_shoot = 2800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
	float Power_shoot = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
	float BaseMass = 0.45f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
	float PrecisionOffset = 0.0f;

	UFUNCTION(BlueprintCallable)
	void RemoveFireBootsEffect();

	UFUNCTION(BlueprintCallable)
	void ActivateFireBootsEffect();


protected:
	virtual void BeginPlay() override;

private:
	bool InitializeShootContext();
	void PerformLineTrace();
	FVector ComputeFinalShootDirection();
	void ApplyImpulse(const FVector& direction);

	AActor* OwnerActor;
	APlayerController* PlayerController;
	float MouseX, MouseY;
	FVector MouseWorldOrigin, MouseWorldDirection;
	FVector RayStart, RayEnd;
	FVector ShootDirection;
	FHitResult HitResult;
	bool bHitSuccess;
};
