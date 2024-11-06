// SPDX-License-Identifier: MIT

#pragma once

#include "CoreTypes/SsDelegates.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SsPlayerItemWidget.generated.h"

class UTextBlock;
class UButton;
struct FSsPlayerData;

UCLASS()
class SPHERESHOOTER_API USsPlayerItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    FOnPlayerItemSelectedSignature OnPlayerItemSelected;
    void SetSelected(bool bIsSelected);

    void SetPlayerData(const FString PlayerName, const FSsPlayerData* PlayerData);

    FString GetPlayerName() const { return PlayerName; }

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> Button;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> NameTextBlock;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> MaxScoreTextBlock;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WhenMaxTextBlock;

    UPROPERTY()
    FString PlayerName;

    virtual void NativeOnInitialized() override;

    UFUNCTION()
    void OnItemClicked();
};
