// LICENSE.md

#pragma once

#include "SphereShooter/Public/UI/MainMenu/SsLevelItemWidget.h"
#include "SphereShooter/Public/CoreTypes/SsLevelData.h"
#include "SphereShooter/Public/SsGameInstance.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SsMainMenuWidget.generated.h"

class UButton;
class UHorizontalBox;


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

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUserWidget> LevelItemWidgetClass;

private:
    UFUNCTION()
    void OnStart();

    UFUNCTION()
    void OnExit();

    UPROPERTY()
    TArray<USsLevelItemWidget*> LevelItemWidgets;

    void InitLevelItems();
    void OnLevelSelected(FSsLevelData* Data);
    USsGameInstance* GetSTUGameInstance() const;
};
