#pragma once
#include "CoreMinimal.h"
#include "SSColor.h"

/**
 * Point top regular hexagon
 */
struct FTile
{
    FTile(uint32 ID, FVector Location, uint8 Row, uint8 Column, bool bIsOutOfRightEdge = false)
        : ID(ID), Location(Location), Row(Row), Column(Column), bIsOutOfRightEdge(bIsOutOfRightEdge)
    {
    }

    uint32 ID{0};
    FVector Location{0, 0, 0};
    uint8 Row{0};
    uint8 Column{0};
    TWeakObjectPtr<class ASSSphere> Ball = nullptr;
    bool bIsOutOfRightEdge = false;
    ESSColor Color = ESSColor::ESSC_Green;

    FTile* L = nullptr;
    FTile* R = nullptr;
    FTile* TL = nullptr;
    FTile* TR = nullptr;
    FTile* BL = nullptr;
    FTile* BR = nullptr;
};