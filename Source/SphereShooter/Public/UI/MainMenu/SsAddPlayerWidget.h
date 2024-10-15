// LICENSE.md

#pragma once

#include "CoreTypes/SsDelegates.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SsAddPlayerWidget.generated.h"

class UEditableTextBox;
class UButton;

UCLASS()
class SPHERESHOOTER_API USsAddPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    FOnPlayerAddSignature OnPlayerAdd;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> PlayerNameTextBox;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> AddButton;

    virtual void NativeOnInitialized() override;

    UFUNCTION()
    void OnAddClicked();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "qq")
    uint8 TextSizeLimit = 16;
};
