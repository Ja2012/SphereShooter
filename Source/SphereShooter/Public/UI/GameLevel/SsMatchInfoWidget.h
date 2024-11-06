// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SsMatchInfoWidget.generated.h"

class UTextBlock;

UCLASS()
class SPHERESHOOTER_API USsMatchInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> PointsTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> MissesTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> RecordPointsTextBlock;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> RecordWhenTextBlock;
};
