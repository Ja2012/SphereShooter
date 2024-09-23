// LICENSE.md

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "SSTaggedActor.generated.h"

UCLASS()
class SPHERESHOOTER_API ASSTaggedActor : public AActor, public IGameplayTagAssetInterface
{
    GENERATED_BODY()
    
public:
    virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Actor)
    FGameplayTagContainer StaticGameplayTags;
};