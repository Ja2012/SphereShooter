// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "SSGameLevelHUD.generated.h"

class USSGameLevelWidget;

UCLASS()
class SPHERESHOOTER_API ASSGameLevelHUD : public AHUD
{
    GENERATED_BODY()

public:
    void ShowGameOverDialog();

protected:
    void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "q")
    TSubclassOf<UUserWidget> GameLevelWidgetClass;

    TObjectPtr<USSGameLevelWidget> GameLevelWidget;
};
