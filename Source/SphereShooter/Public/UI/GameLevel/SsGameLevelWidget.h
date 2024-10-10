// LICENSE.md

#pragma once

#include "CoreTypes/SsDelegates.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SsGameLevelWidget.generated.h"

class UButton;
class USsConfirmWidget;

UCLASS()
class SPHERESHOOTER_API USsGameLevelWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void ShowGameOverDialog();

    FOnExitLevelClickedSignature OnExitClicked;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USsConfirmWidget> ExitDialog;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USsConfirmWidget> GameOverDialog;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnExitButtonClicked();

    void OnExitDialogResolve(const bool bIsOkayPressed);
};
