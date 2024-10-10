// LICENSE.md


#include "CoreTypes/SsTaggedActor.h"

void ASsTaggedActor::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
    TagContainer.AppendTags(StaticGameplayTags);
}
