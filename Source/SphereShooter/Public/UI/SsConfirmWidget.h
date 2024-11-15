// SPDX-License-Identifier: MIT

#pragma once

#include "CoreTypes/SsDelegates.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SsConfirmWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class SPHERESHOOTER_API USsConfirmWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    FOnConfirmClickedSignature OnConfirmVariantClicked;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> PositiveButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> NegativeButton;

    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Title;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> Message;

private:
    UFUNCTION()
    void OnPositiveClicked();

    UFUNCTION()
    void OnNegativeClicked();
};
