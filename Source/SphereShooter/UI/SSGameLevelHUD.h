// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "SSGameLevelHUD.generated.h"

UCLASS()
class SPHERESHOOTER_API ASSGameLevelHUD : public AHUD
{
    GENERATED_BODY()

public:
    UUserWidget* GetGameLevelWidget() const { return GameLevelWidget; }

protected:
    void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "q")
    TSubclassOf<UUserWidget> GameLevelWidgetClass;

    TObjectPtr<UUserWidget> GameLevelWidget;
};
