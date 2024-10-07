// LICENSE.md

#include "UI/SSConfirmWidget.h"

#include "Components/Button.h"

void USSConfirmWidget::NativeOnInitialized()
{
    if (PositiveButton)
    {
        PositiveButton->OnClicked.AddDynamic(this, &USSConfirmWidget::OnPositiveClicked);
    }
    if (NegativeButton)
    {
        NegativeButton->OnClicked.AddDynamic(this, &USSConfirmWidget::OnNegativeClicked);
    }
}

void USSConfirmWidget::OnPositiveClicked()
{
    OnConfirmVariantClicked.Broadcast(true);
}

void USSConfirmWidget::OnNegativeClicked()
{
    OnConfirmVariantClicked.Broadcast(false);
}
