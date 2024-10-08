// LICENSE.md


#include "SphereShooter/Public/CoreTypes/SsTaggedActor.h"

void ASsTaggedActor::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
    TagContainer.AppendTags(StaticGameplayTags);
}
