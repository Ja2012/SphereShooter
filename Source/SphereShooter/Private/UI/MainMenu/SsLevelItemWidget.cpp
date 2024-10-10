// LICENSE.md

#include "UI/MainMenu/SsLevelItemWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USsLevelItemWidget::SetLevelData(const FSsLevelData& Data)
{
    LevelData = Data;
    if (LevelNameTextBlock)
    {
        LevelNameTextBlock->SetText(FText::FromName(Data.LevelDisplayName));
    }
    if (LevelImage)
    {
        LevelImage->SetBrushFromTexture(Data.LevelThumb);
    }
}

void USsLevelItemWidget::SetSelected(bool IsSelected) 
{
    if (FrameImage)
    {
        FrameImage->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void USsLevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &USsLevelItemWidget::OnLevelItemClicked);
    }
}

void USsLevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(&LevelData);
}
