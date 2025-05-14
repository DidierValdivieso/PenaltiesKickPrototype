#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PorteriaActor.generated.h"

UCLASS()
class PENALTIESKICK_API APorteriaActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APorteriaActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PorteriaMesh;

protected:
	virtual void BeginPlay() override;

};
