#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Widgets/GameStatsWidget.h"
#include "PenaltiesKickGameMode.generated.h"

class APorteroCharacter;
class APateadorCharacter;

UCLASS()
class PENALTIESKICK_API APenaltiesKickGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APenaltiesKickGameMode();

	void BeginPlay();
	void RegisterShotResult(bool bWasGoal);
	void DeactivateFireBootsEffect();
	bool bWaitingNextShot = false;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentShot = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 GoalsScored = 0;

	UPROPERTY(BlueprintReadOnly)
	int32 MoralPoints = 0;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxShots = 5;

	UFUNCTION(BlueprintCallable)
	int32 GetMoralPoints() const;

	UFUNCTION(BlueprintCallable)
	void SpendMoralPoints(int32 Amount);


	UPROPERTY()
	ABallActor* BallReference;

	UPROPERTY()
	UGameStatsWidget* GameplayWidget;

	UPROPERTY()
	ABallActor* CurrentBall;

	UPROPERTY()
	APateadorCharacter* CurrentPateador;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	APorteroCharacter* SpawnPorteroCharacter();

	// Funcion para spawnear el Pateador basado en una ubicacion
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	APateadorCharacter* SpawnPateadorCharacter(const FVector& ReferenceLocation);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	ABallActor* SpawnBall(const FVector& PateadorLocation);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	APorteriaActor* SpawnPorteria();

	UFUNCTION(BlueprintCallable)
	void ResetLevelComplete();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void ReubicationSpawnBall();

	// Clase del personaje que vamos a spawnear
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<APorteroCharacter> PorteroCharacterClass;

	// Clase del PateadorCharacter para spawnear
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<APateadorCharacter> PateadorCharacterClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<ABallActor> BallClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<APorteriaActor> PorteriaClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> GameplayWidgetClass;
};
