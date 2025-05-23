#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStatsWidget.generated.h"

UCLASS()
class PENALTIESKICK_API UGameStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void UpdateStats(int32 Goles, int32 Turno, int32 Moral);

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GoalsText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TurnText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MoralText;
	
};
