// LICENSE.md

#pragma once

#include "Auxiliary/SSDelegates.h"
#include "Auxiliary/SSLevelData.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SSLevelItemWidget.generated.h"

class UTextBlock;
class UButton;
class UImage;

UCLASS()
class SPHERESHOOTER_API USSLevelItemWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    FOnLevelSelectedSignature OnLevelSelected;
    void SetLevelData(const FSSLevelData& Data);
    FSSLevelData GetLevelData() const { return LevelData; };

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
    FSSLevelData LevelData;

    UFUNCTION()
    void OnLevelItemClicked();
};
