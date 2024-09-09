#pragma once
#include "CoreMinimal.h"
#include "SSColor.h"
#include "SSSphere.h"

/**
 * Point top regular hexagon
 */
struct FTile
{
    FTile(uint32 ID, FVector Location, uint8 Row, uint8 Column, bool bIsOutOfRightEdge = false)
        : ID(ID), Location(Location), Row(Row), Column(Column), bIsOutOfRightEdge(bIsOutOfRightEdge)
    {
    }

    void Reset()
    {
        Ball = nullptr;
        Color = ESSColor::ESSC_NoColor;
    }
    void Set(ASSSphere* InBall)
    {
        Ball = InBall;
        Ball->Tile = this;
        Color = InBall->Color;
    }

    uint32 ID{0};
    FVector Location{0, 0, 0};
    uint8 Row{0};
    uint8 Column{0};
    TWeakObjectPtr<class ASSSphere> Ball = nullptr;
    bool bIsOutOfRightEdge = false;
    ESSColor Color = ESSColor::ESSC_NoColor;

    FTile* Left = nullptr;
    FTile* Right = nullptr;
    FTile* TopLeft = nullptr;
    FTile* TopRight = nullptr;
    FTile* BottomLeft = nullptr;
    FTile* BottomRight = nullptr;

    TArray<FTile*> Neighbors {};
};