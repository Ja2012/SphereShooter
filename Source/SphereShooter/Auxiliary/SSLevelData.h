// LICENSE.md

#pragma once

#include "CoreMinimal.h"

#include "SSLevelData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSSLevelData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "q")
	FName LevelName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "q")
	FName LevelDisplayName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "q")
	TObjectPtr<UTexture2D> LevelThumb;

};
