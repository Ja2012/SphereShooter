// SPDX-License-Identifier: MIT


#include "UI/MainMenu/SsPlayerItemWidget.h"

#include "SsUtils.h"
#include "CoreTypes/SsSaveGame.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void USsPlayerItemWidget::SetSelected(bool bIsSelected)
{
    if (bIsSelected)
    {
        Button->SetBackgroundColor(FLinearColor(.5f, .5f, 1));
    }
    else
    {
        Button->SetBackgroundColor(FLinearColor::White);
    }
}

void USsPlayerItemWidget::SetPlayerData(const FString InPlayerName, const FSsPlayerData* PlayerData)
{
    PlayerName = InPlayerName;
    NameTextBlock->SetText(FText::FromString(PlayerName));
    MaxScoreTextBlock->SetText(FText::AsNumber(PlayerData->MaxScore));
    //MaxScoreTextBlock->SetText(SsUtils::TextFromInt(PlayerData->MaxScore));
    WhenMaxTextBlock->SetText(FText::FromString(PlayerData->WhenMaxScore.ToFormattedString(TEXT("%d.%m.%y"))));
    
    //NameTextBlock->SetText(FText::(PlayerName));
}

void USsPlayerItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Button->OnClicked.AddDynamic(this, &USsPlayerItemWidget::OnItemClicked);
}

void USsPlayerItemWidget::OnItemClicked()
{
    Button->SetBackgroundColor(FLinearColor::Blue);
    OnPlayerItemSelected.Broadcast(PlayerName);
}
