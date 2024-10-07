// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSGameLevelWidget.generated.h"

class UButton;
class USSConfirmWidget;

UCLASS()
class SPHERESHOOTER_API USSGameLevelWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USSConfirmWidget> ExitDialog;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnExitButtonClicked();

    void OnExitDialogResolve(const bool bIsOkayPressed);
};
