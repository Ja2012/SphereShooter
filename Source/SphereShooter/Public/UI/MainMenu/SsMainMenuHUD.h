// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "SsMainMenuHUD.generated.h"

class USsMainMenuWidget;

UCLASS()
class SPHERESHOOTER_API ASsMainMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "q")
	TSubclassOf<UUserWidget> MenuWidgetClass;	

    UPROPERTY()
    TObjectPtr<USsMainMenuWidget> MainMenuWidget;
};
