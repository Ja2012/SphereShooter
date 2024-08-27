// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BallType.generated.h"

/**
 *
 */
UCLASS()
class SPHERESHOOTER_API UBallType : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UMaterial> Material;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<class ASSSphere> SphereClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CollisionDiameter;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MeshDiameter;
};
