#include "UI/CustomRoundedBoxBrush.h"

FCustomRoundedBoxBrush::FCustomRoundedBoxBrush()
{
    DrawAs = ESlateBrushDrawType::RoundedBox;
    
    // Initialize border settings
    bUseOutline = false;
    BorderWidth = FVector4f(1.0f, 1.0f, 1.0f, 1.0f);
    OutlineColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Setup base FSlateBrush parameters
    Tiling = ESlateBrushTileType::NoTile;
    Mirroring = ESlateBrushMirrorType::NoMirror;
    TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
} 