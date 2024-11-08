#pragma once

#include "CoreMinimal.h"
#include "Rendering/DrawElements.h"
#include "Framework/Application/SlateApplication.h"
#include "UI/CustomRoundedBoxBrush.h"

enum class ECornerPosition
{
    TopLeft,
    TopRight,
    BottomRight,
    BottomLeft
};

class FRoundedCornerElement
{
public:
    FRoundedCornerElement(
        const FGeometry& InGeometry,
        float InCornerRadius,
        const FLinearColor& InOutlineColor,
        ECornerPosition InCornerPosition,
        const FCustomRoundedBoxBrush& InBrush
    );

    void DrawRoundedCorner(
        const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect,
        FSlateWindowElementList& OutDrawElements,
        int32 LayerId) const;

private:
    float CornerRadius;
    FLinearColor OutlineColor;
    ECornerPosition CornerPosition;
    const FCustomRoundedBoxBrush& Brush;
}; 