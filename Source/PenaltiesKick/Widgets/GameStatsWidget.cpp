#include "GameStatsWidget.h"
#include "Components/TextBlock.h"

void UGameStatsWidget::UpdateStats(int32 Goles, int32 Turno, int32 Moral)
{
    if (GolesText)
        GolesText->SetText(FText::Format(FText::FromString("Goles: {0}"), FText::AsNumber(Goles)));

    if (TurnoText)
        TurnoText->SetText(FText::Format(FText::FromString("Turno: {0}"), FText::AsNumber(Turno)));

    if (MoralText)
        MoralText->SetText(FText::Format(FText::FromString("Moral: {0}"), FText::AsNumber(Moral)));
}


