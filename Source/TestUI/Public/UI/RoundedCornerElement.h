#pragma once

#include "CoreMinimal.h"
#include "Rendering/DrawElements.h"
#include "Framework/Application/SlateApplication.h"
#include "UI/CustomRoundedBoxBrush.h"

enum class ESide
{
    Top,
    Right,
    Bottom,
    Left
};

class FRoundedCornerElement
{
public:
    FRoundedCornerElement(
        const FGeometry& InGeometry,
        float InCornerRadius,
        float InBorderWidth,
        const FLinearColor& InOutlineColor,
        bool bIsRightCorner,
        ESide InSide,
        const FCustomRoundedBoxBrush& InBrush
    );

    void DrawRoundedCorner(
        const FGeometry& AllottedGeometry,
        const FSlateRect& MyCullingRect,
        FSlateWindowElementList& OutDrawElements,
        int32 LayerId) const;

private:
    float CornerRadius;
    float BorderWidth;
    FLinearColor OutlineColor;
    bool bRightCorner;
    ESide Side;
    const FCustomRoundedBoxBrush& Brush;
}; 