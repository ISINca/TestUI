#include "UI/CustomRoundedBoxBrush.h"

FCustomRoundedBoxBrush::FCustomRoundedBoxBrush()
{
    DrawAs = ESlateBrushDrawType::RoundedBox;
    
    // Инициализация базовых параметров закругления
    CornerRadius = 12.0f;
    CornerRadii = FVector4f(12.0f, 12.0f, 12.0f, 12.0f);

    // Инициализация параметров обводки
    OutlineColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f); // Черный цвет
    bUseOutline = false;
    bUseUniformOutlineWidth = true;
    OutlineWidth = 1.0f;
    OutlineWidths = FVector4f(1.0f, 1.0f, 1.0f, 1.0f);

    // Инициализация дополнительных эффектов
    Opacity = 1.0f;
    bUseSoftEdges = false;
    SoftEdgeSize = 0.0f;

    // Настройка базовых параметров FSlateBrush
    Tiling = ESlateBrushTileType::NoTile;
    Mirroring = ESlateBrushMirrorType::NoMirror;
    ImageSize = FVector2D(32.0f, 32.0f);
    Margin = FMargin(0.0f);
} 