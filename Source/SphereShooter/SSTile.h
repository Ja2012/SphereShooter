#pragma once
#include "CoreMinimal.h"
#include "SSColor.h"
#include "SSSphere.h"

#include <unordered_set>

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

    bool Empty() const { return Ball == nullptr; };

    uint32 ID{0};
    TWeakObjectPtr<class ASSSphere> Ball = nullptr;
    ESSColor Color = ESSColor::ESSC_NoColor;
    uint8 Row{0};
    uint8 Column{0};
    FVector Location{0, 0, 0};
    bool bIsOutOfRightEdge = false;

    FTile* Left = nullptr;
    FTile* Right = nullptr;
    FTile* TopLeft = nullptr;
    FTile* TopRight = nullptr;
    FTile* BottomLeft = nullptr;
    FTile* BottomRight = nullptr;

    std::unordered_set<FTile*> Neighbors{};

};

inline bool operator<(const FTile& A, const FTile& B) { return A.Row < B.Row; }

inline bool operator==(const FTile& A, const FTile& B)
{
    return std::tie(A.ID, A.Ball, A.Color, A.Row, A.Column) == std::tie(B.ID, B.Ball, B.Color, B.Row, B.Column);
}


/* implement hash function so we can put FTile into an unordered_map as key */
template <>
struct std::hash<FTile>
{
    std::size_t operator()(const FTile& Tile) const noexcept
    {
        const std::size_t HashID = std::hash<int>()(Tile.ID);
        const std::size_t HashRow = std::hash<int>()(Tile.Row);
        const std::size_t HashColumn = std::hash<int>()(Tile.Column);
        const std::size_t HashColor = std::hash<int>()((int)Tile.Color);
        return HashID ^ (HashRow << 1) ^ (HashColumn << 2) ^ (HashColor << 3);
    }
};