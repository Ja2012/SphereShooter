// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "SsGameLevelHUD.generated.h"

class USsGameLevelWidget;

UCLASS()
class SPHERESHOOTER_API ASsGameLevelHUD : public AHUD
{
    GENERATED_BODY()

public:
    void ShowGameOverDialog();

protected:
    void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "q")
    TSubclassOf<UUserWidget> GameLevelWidgetClass;

    TObjectPtr<USsGameLevelWidget> GameLevelWidget;
};
