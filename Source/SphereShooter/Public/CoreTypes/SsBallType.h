// LICENSE.md

#pragma once

#include "CoreTypes/SsColor.h"

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "SsBallType.generated.h"

UCLASS()
class SPHERESHOOTER_API USsBallType : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<ESsColor, TObjectPtr<UMaterialInstance>> MaterialInstances;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class ASsBaseSphere> SphereClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TargetCollisionDiameter;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float TargetMeshDiameter;
};
