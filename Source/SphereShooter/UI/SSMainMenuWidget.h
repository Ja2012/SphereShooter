// LICENSE.md

#pragma once

#include "SSLevelItemWidget.h"
#include "Auxiliary/SSLevelData.h"
#include "SSGameInstance.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SSMainMenuWidget.generated.h"

class UButton;
class UHorizontalBox;


UCLASS()
class SPHERESHOOTER_API USSMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> StartButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* LevelItemsBox;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

private:
    UFUNCTION()
    void OnStart();

    UFUNCTION()
    void OnExit();

    UPROPERTY()
    TArray<USSLevelItemWidget*> LevelItemWidgets;

    void InitLevelItems();
    void OnLevelSelected(FSSLevelData* Data);
    USSGameInstance* GetSTUGameInstance() const;
};
