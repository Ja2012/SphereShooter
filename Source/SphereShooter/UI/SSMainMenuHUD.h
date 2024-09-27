// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SSMainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPHERESHOOTER_API ASSMainMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "q")
	TSubclassOf<UUserWidget> MenuWidgetClass;	
};
