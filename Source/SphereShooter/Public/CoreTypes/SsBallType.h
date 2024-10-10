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
    TObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<ESsColor, TObjectPtr<UMaterialInstance>> MaterialInstances;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class ASsSphere> SphereClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CollisionDiameter;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MeshDiameter;
};
