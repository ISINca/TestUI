#include "UI/SCustomRoundedBox.h"
#include "SlateOptMacros.h"
#include "Rendering/DrawElements.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCustomRoundedBox::Construct(const FArguments& InArgs)
{
    Brush = InArgs._Brush;

    ChildSlot
    [
        InArgs._Content.Widget
    ];
}

void SCustomRoundedBox::SetContent(const TSharedRef<SWidget>& InContent)
{
    ChildSlot
    [
        InContent
    ];
}

int32 SCustomRoundedBox::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
    const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
    int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    // Отрисовка основного фона
    DrawRoundedBox(OutDrawElements, AllottedGeometry, LayerId);

    // Отрисовка обводки, если она включена
    if (Brush.bUseOutline)
    {
        DrawOutline(OutDrawElements, AllottedGeometry, LayerId + 1);
    }

    // Отрисовка дочерних элементов
    return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, 
        LayerId + 2, InWidgetStyle, bParentEnabled);
}

void SCustomRoundedBox::SetBrush(const FCustomRoundedBoxBrush& InBrush)
{
    Brush = InBrush;
}

void SCustomRoundedBox::DrawRoundedBox(FSlateWindowElementList& OutDrawElements, 
    const FGeometry& AllottedGeometry, int32 LayerId) const
{
    const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
    
    // Create box geometry
    FSlateDrawElement::MakeBox(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(),
        &Brush,
        ESlateDrawEffect::None,
        Brush.TintColor.GetSpecifiedColor()
    );
}

void SCustomRoundedBox::DrawOutline(FSlateWindowElementList& OutDrawElements, 
    const FGeometry& AllottedGeometry, int32 LayerId) const
{
    const FVector2D LocalSize = AllottedGeometry.GetLocalSize();
    
    FSlateBrush OutlineBrush;
    OutlineBrush.DrawAs = ESlateBrushDrawType::Box;
    
    // Top border
    if (Brush.GetTopBorderWidth() > 0)
    {
        const float StartX = Brush.OutlineSettings.CornerRadii.X; // TopLeft
        const float EndX = LocalSize.X - Brush.OutlineSettings.CornerRadii.Y; // TopRight
        
        if (EndX > StartX)
        {
            FSlateDrawElement::MakeBox(
                OutDrawElements,
                LayerId,
                AllottedGeometry.ToPaintGeometry(
                    FVector2D(StartX, 0),
                    FVector2D(EndX - StartX, Brush.GetTopBorderWidth())
                ),
                &OutlineBrush,
                ESlateDrawEffect::None,
                Brush.OutlineColor
            );
        }
    }

    // Right border
    if (Brush.GetRightBorderWidth() > 0)
    {
        const float StartY = Brush.OutlineSettings.CornerRadii.Y; // TopRight
        const float EndY = LocalSize.Y - Brush.OutlineSettings.CornerRadii.Z; // BottomRight
        
        if (EndY > StartY)
        {
            FSlateDrawElement::MakeBox(
                OutDrawElements,
                LayerId,
                AllottedGeometry.ToPaintGeometry(
                    FVector2D(LocalSize.X - Brush.GetRightBorderWidth(), StartY),
                    FVector2D(Brush.GetRightBorderWidth(), EndY - StartY)
                ),
                &OutlineBrush,
                ESlateDrawEffect::None,
                Brush.OutlineColor
            );
        }
    }

    // Bottom border
    if (Brush.GetBottomBorderWidth() > 0)
    {
        const float StartX = Brush.OutlineSettings.CornerRadii.W; // BottomLeft
        const float EndX = LocalSize.X - Brush.OutlineSettings.CornerRadii.Z; // BottomRight
        
        if (EndX > StartX)
        {
            FSlateDrawElement::MakeBox(
                OutDrawElements,
                LayerId,
                AllottedGeometry.ToPaintGeometry(
                    FVector2D(StartX, LocalSize.Y - Brush.GetBottomBorderWidth()),
                    FVector2D(EndX - StartX, Brush.GetBottomBorderWidth())
                ),
                &OutlineBrush,
                ESlateDrawEffect::None,
                Brush.OutlineColor
            );
        }
    }

    // Left border
    if (Brush.GetLeftBorderWidth() > 0)
    {
        const float StartY = Brush.OutlineSettings.CornerRadii.X; // TopLeft
        const float EndY = LocalSize.Y - Brush.OutlineSettings.CornerRadii.W; // BottomLeft
        
        if (EndY > StartY)
        {
            FSlateDrawElement::MakeBox(
                OutDrawElements,
                LayerId,
                AllottedGeometry.ToPaintGeometry(
                    FVector2D(0, StartY),
                    FVector2D(Brush.GetLeftBorderWidth(), EndY - StartY)
                ),
                &OutlineBrush,
                ESlateDrawEffect::None,
                Brush.OutlineColor
            );
        }
    }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION 