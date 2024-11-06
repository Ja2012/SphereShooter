// SPDX-License-Identifier: MIT

#include "UI/GameLevel/SsGameLevelHUD.h"
#include "UI/GameLevel/SsGameLevelWidget.h"
#include "UI/SsConfirmWidget.h"
#include "UI/GameLevel/SsMatchInfoWidget.h"
#include "Components/TextBlock.h"
#include "Player/SsPlayerState.h"
#include "SsUtils.h"

void ASsGameLevelHUD::ShowGameOverDialog()
{
    GameLevelWidget->GetGameOverDialog()->SetVisibility(ESlateVisibility::Visible);
}

void ASsGameLevelHUD::UpdateMatchInfo(const ASsPlayerState* PlayerState) const
{
    USsMatchInfoWidget* Widget = GameLevelWidget->GetMatchInfoWidget();
    Widget->PointsTextBlock->SetText(SsUtils::TextFromInt(PlayerState->GetScore()));
    Widget->MissesTextBlock->SetText(SsUtils::TextFromInt(PlayerState->GetMissesCount()));
    Widget->RecordPointsTextBlock->SetText(SsUtils::TextFromInt(PlayerState->GetMaxScore()));
    Widget->RecordWhenTextBlock->SetText(FText::FromString(PlayerState->GetWhenMaxScore().ToString()));
}

// USsGameLevelWidget
void ASsGameLevelHUD::BeginPlay()
{
    Super::BeginPlay();
    if (GameLevelWidgetClass)
    {
        GameLevelWidget = CreateWidget<USsGameLevelWidget>(GetWorld(), GameLevelWidgetClass);
        if (GameLevelWidget)
        {
            GameLevelWidget->AddToViewport();
        }
    }
}