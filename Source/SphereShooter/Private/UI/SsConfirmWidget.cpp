// SPDX-License-Identifier: MIT

#include "UI/SsConfirmWidget.h"

#include "Components/Button.h"

void USsConfirmWidget::NativeOnInitialized()
{
    if (PositiveButton)
    {
        PositiveButton->OnClicked.AddDynamic(this, &USsConfirmWidget::OnPositiveClicked);
    }
    if (NegativeButton)
    {
        NegativeButton->OnClicked.AddDynamic(this, &USsConfirmWidget::OnNegativeClicked);
    }
}

void USsConfirmWidget::OnPositiveClicked()
{
    OnConfirmVariantClicked.Broadcast(true);
}

void USsConfirmWidget::OnNegativeClicked()
{
    OnConfirmVariantClicked.Broadcast(false);
}
