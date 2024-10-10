// LICENSE.md

#pragma once

#include "CoreTypes/SsDelegates.h"
#include "CoreTypes/SsLevelData.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SsLevelItemWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;

UCLASS()
class SPHERESHOOTER_API USsLevelItemWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    FOnLevelSelectedSignature OnLevelSelected;
    void SetLevelData(const FSsLevelData& Data);
    FSsLevelData GetLevelData() const { return LevelData; };

    void SetSelected(bool IsSelected);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> LevelSelectButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> LevelNameTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> LevelImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> FrameImage;

    virtual void NativeOnInitialized() override;

private:
    FSsLevelData LevelData;

    UFUNCTION()
    void OnLevelItemClicked();
};
