#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallActor.h"
#include "CardItemActor.generated.h"

UCLASS()
class PENALTIESKICK_API ACardItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACardItemActor();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CardItemMesh;

protected:
	virtual void BeginPlay() override;
};
