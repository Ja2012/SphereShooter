// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SSMainMenuWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class SPHERESHOOTER_API USSMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

private:
	UFUNCTION()
	void OnStart();

	UFUNCTION()
	void OnExit();
};
