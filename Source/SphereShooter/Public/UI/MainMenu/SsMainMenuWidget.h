// LICENSE.md

#pragma once

#include "UI/MainMenu/SsLevelItemWidget.h"
#include "UI/MainMenu/SsPlayerItemWidget.h"
#include "CoreTypes/SsLevelData.h"
#include "SsGameInstance.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SsMainMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class UVerticalBox;
class USsAddPlayerWidget;

UCLASS()
class SPHERESHOOTER_API USsMainMenuWidget : public UUserWidget
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

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UVerticalBox> PlayerItemsBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USsAddPlayerWidget> AddPlayerWidget;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> PlayerItemWidgetClass;


private:
    void InitLevelItems();
    void InitPlayerItems();

    UFUNCTION()
    void OnStart();

    UFUNCTION()
    void OnExit();

    UPROPERTY()
    TArray<USsLevelItemWidget*> LevelItemWidgets;
    UPROPERTY()
    TArray<USsPlayerItemWidget*> PlayerItemWidgets;

    void OnLevelSelected(FSsLevelData* Data);
    void OnPlayerSelected(FString PlayerName);
    void OnAddPlayer(FString PlayerName);
    USsGameInstance* GetGameInstance() const;
};
