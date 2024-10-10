// LICENSE.md

#pragma once

#include "CoreTypes/SsDelegates.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SsGameLevelWidget.generated.h"

class UButton;
class USsConfirmWidget;
class USsMatchInfoWidget;

UCLASS()
class SPHERESHOOTER_API USsGameLevelWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void ShowGameOverDialog();

    FOnExitLevelClickedSignature OnExitClicked;

    TObjectPtr<USsConfirmWidget> GetGameOverDialog() const { return GameOverDialog; }
    TObjectPtr<USsMatchInfoWidget> GetMatchInfoWidget() const { return MatchInfoWidget; }

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USsConfirmWidget> ExitDialog;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USsConfirmWidget> GameOverDialog;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USsMatchInfoWidget> MatchInfoWidget;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnExitButtonClicked();

    void OnExitDialogResolve(const bool bIsOkayPressed);
};
