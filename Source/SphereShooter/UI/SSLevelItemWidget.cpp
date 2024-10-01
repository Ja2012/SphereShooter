// LICENSE.md

#include "SSLevelItemWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USSLevelItemWidget::SetLevelData(const FSSLevelData& Data)
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

void USSLevelItemWidget::SetSelected(bool IsSelected) 
{
    if (FrameImage)
    {
        FrameImage->SetVisibility(IsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void USSLevelItemWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (LevelSelectButton)
    {
        LevelSelectButton->OnClicked.AddDynamic(this, &USSLevelItemWidget::OnLevelItemClicked);
    }
}

void USSLevelItemWidget::OnLevelItemClicked()
{
    OnLevelSelected.Broadcast(&LevelData);
}
