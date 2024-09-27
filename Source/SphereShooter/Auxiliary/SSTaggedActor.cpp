// LICENSE.md


#include "SSTaggedActor.h"

void ASSTaggedActor::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
    TagContainer.AppendTags(StaticGameplayTags);
}
