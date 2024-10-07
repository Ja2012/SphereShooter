// LICENSE.md

#include "UI/SSGameLevelWidget.h"
#include "UI/SSConfirmWidget.h"
#include "GameLevel/SSGameLevelPlayerController.h"
#include "SSGameInstance.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USSGameLevelWidget::ShowGameOverDialog() 
{
    GameOverDialog->SetVisibility(ESlateVisibility::Visible);
}

void USSGameLevelWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &USSGameLevelWidget::OnExitButtonClicked);
    }
    if (ExitDialog)
    {
        ExitDialog->OnConfirmVariantClicked.AddUObject(this, &USSGameLevelWidget::OnExitDialogResolve);
    }   
    if (GameOverDialog)
    {
        GameOverDialog->OnConfirmVariantClicked.AddUObject(this, &USSGameLevelWidget::OnExitDialogResolve);
    }
}

void USSGameLevelWidget::OnExitButtonClicked()
{
    UGameplayStatics::SetGamePaused(GetWorld(), true);
    if (ExitDialog) ExitDialog->SetVisibility(ESlateVisibility::Visible);
    ASSGameLevelPlayerController* SSPlayerController = GetOwningPlayer<ASSGameLevelPlayerController>();
    if (SSPlayerController) SSPlayerController->SetInputMode(FInputModeUIOnly());
}

void USSGameLevelWidget::OnExitDialogResolve(const bool bIsOkayPressed)
{
    if (bIsOkayPressed)
    {

        UGameplayStatics::OpenLevel(this, GetWorld()->GetGameInstance<USSGameInstance>()->GetMainMenuLevelName());
    }
    else
    {
        ASSGameLevelPlayerController* SSPlayerController = GetOwningPlayer<ASSGameLevelPlayerController>();
        if (SSPlayerController)
        {
            SSPlayerController->SetInputMode(FInputModeGameAndUI());
            ExitDialog->SetVisibility(ESlateVisibility::Hidden);
            UGameplayStatics::SetGamePaused(GetWorld(), false);

        }
    }
}
