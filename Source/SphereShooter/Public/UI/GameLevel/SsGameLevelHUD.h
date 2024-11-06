// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "SsGameLevelHUD.generated.h"

class USsGameLevelWidget;
class ASsPlayerState;

UCLASS()
class SPHERESHOOTER_API ASsGameLevelHUD : public AHUD
{
    GENERATED_BODY()

public:
    TObjectPtr<USsGameLevelWidget> GetWidget() const { return GameLevelWidget; }

    void ShowGameOverDialog();
    void UpdateMatchInfo(const ASsPlayerState* PlayerState) const;

protected:
    void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "q")
    TSubclassOf<UUserWidget> GameLevelWidgetClass;

    UPROPERTY()
    TObjectPtr<USsGameLevelWidget> GameLevelWidget;
};
