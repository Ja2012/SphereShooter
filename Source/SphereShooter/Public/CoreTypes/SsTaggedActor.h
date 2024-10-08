// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagAssetInterface.h"

#include "SsTaggedActor.generated.h"

UCLASS()
class SPHERESHOOTER_API ASsTaggedActor : public AActor, public IGameplayTagAssetInterface
{
    GENERATED_BODY()
    
public:
    virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Actor)
    FGameplayTagContainer StaticGameplayTags;
};