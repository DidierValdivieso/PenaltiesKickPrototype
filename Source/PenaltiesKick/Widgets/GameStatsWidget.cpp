#include "GameStatsWidget.h"
#include "Components/TextBlock.h"

void UGameStatsWidget::UpdateStats(int32 Goals, int32 Turn, int32 Moral)
{
    if (GoalsText)
        GoalsText->SetText(FText::Format(FText::FromString("Goals: {0}"), FText::AsNumber(Goals)));

    if (TurnText)
        TurnText->SetText(FText::Format(FText::FromString("Turn: {0}"), FText::AsNumber(Turn)));

    if (MoralText)
        MoralText->SetText(FText::Format(FText::FromString("Moral: {0}"), FText::AsNumber(Moral)));
}


