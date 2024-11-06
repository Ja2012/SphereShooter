// SPDX-License-Identifier: MIT

#include "UI/GameLevel/SsGameLevelWidget.h"

#include "UI/SsConfirmWidget.h"
#include "Player/SsGameLevelPlayerController.h"
#include "SsGameInstance.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USsGameLevelWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &USsGameLevelWidget::OnExitButtonClicked);
    }
    if (ExitDialog)
    {
        ExitDialog->OnConfirmVariantClicked.AddUObject(this, &USsGameLevelWidget::OnExitDialogResolve);
    }
    if (GameOverDialog)
    {
        GameOverDialog->OnConfirmVariantClicked.AddUObject(this, &USsGameLevelWidget::OnExitDialogResolve);
    }
}

void USsGameLevelWidget::OnExitButtonClicked()
{
    UGameplayStatics::SetGamePaused(GetWorld(), true);
    if (ExitDialog) ExitDialog->SetVisibility(ESlateVisibility::Visible);
    ASsGameLevelPlayerController* SSPlayerController = GetOwningPlayer<ASsGameLevelPlayerController>();
    if (SSPlayerController) SSPlayerController->SetInputMode(FInputModeUIOnly());
}

void USsGameLevelWidget::OnExitDialogResolve(const bool bIsOkayPressed)
{
    if (bIsOkayPressed)
    {
        OnExitClicked.Broadcast();
    }
    else
    {
        ASsGameLevelPlayerController* SSPlayerController = GetOwningPlayer<ASsGameLevelPlayerController>();
        if (SSPlayerController)
        {
            SSPlayerController->SetInputMode(FInputModeGameAndUI());
            ExitDialog->SetVisibility(ESlateVisibility::Hidden);
            UGameplayStatics::SetGamePaused(GetWorld(), false);
        }
    }
}
