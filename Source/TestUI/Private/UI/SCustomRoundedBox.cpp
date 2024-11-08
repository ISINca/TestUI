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
    
    // Отрисовка прямых участков обводки
    
    // Top border
    if (Brush.GetTopBorderWidth() > 0)
    {
        const float StartX = Brush.OutlineSettings.CornerRadii.X;
        const float EndX = LocalSize.X - Brush.OutlineSettings.CornerRadii.Y;
        
        if (EndX > StartX)
        {
            const FVector2D Position(StartX, 0);
            const FVector2D Size(EndX - StartX, Brush.GetTopBorderWidth());
            
            FSlateDrawElement::MakeBox(
                OutDrawElements,
                LayerId,
                AllottedGeometry.ToPaintGeometry(
                    FVector2f(Size),
                    FSlateLayoutTransform(Position)
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
        const float StartY = Brush.OutlineSettings.CornerRadii.Y;
        const float EndY = LocalSize.Y - Brush.OutlineSettings.CornerRadii.Z;
        
        if (EndY > StartY)
        {
            const FVector2D Position(LocalSize.X - Brush.GetRightBorderWidth(), StartY);
            const FVector2D Size(Brush.GetRightBorderWidth(), EndY - StartY);
            
            FSlateDrawElement::MakeBox(
                OutDrawElements,
                LayerId,
                AllottedGeometry.ToPaintGeometry(
                    FVector2f(Size),
                    FSlateLayoutTransform(Position)
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
        const float StartX = Brush.OutlineSettings.CornerRadii.W;
        const float EndX = LocalSize.X - Brush.OutlineSettings.CornerRadii.Z;
        
        if (EndX > StartX)
        {
            const FVector2D Position(StartX, LocalSize.Y - Brush.GetBottomBorderWidth());
            const FVector2D Size(EndX - StartX, Brush.GetBottomBorderWidth());
            
            FSlateDrawElement::MakeBox(
                OutDrawElements,
                LayerId,
                AllottedGeometry.ToPaintGeometry(
                    FVector2f(Size),
                    FSlateLayoutTransform(Position)
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
        const float StartY = Brush.OutlineSettings.CornerRadii.X;
        const float EndY = LocalSize.Y - Brush.OutlineSettings.CornerRadii.W;
        
        if (EndY > StartY)
        {
            const FVector2D Position(0, StartY);
            const FVector2D Size(Brush.GetLeftBorderWidth(), EndY - StartY);
            
            FSlateDrawElement::MakeBox(
                OutDrawElements,
                LayerId,
                AllottedGeometry.ToPaintGeometry(
                    FVector2f(Size),
                    FSlateLayoutTransform(Position)
                ),
                &OutlineBrush,
                ESlateDrawEffect::None,
                Brush.OutlineColor
            );
        }
    }

    // Отрисовка угловых элементов
    // Верхний левый угол
    DrawCornerOutline(OutDrawElements, AllottedGeometry, LayerId, ECornerPosition::TopLeft);
    
    // Верхний правый угол
    DrawCornerOutline(OutDrawElements, AllottedGeometry, LayerId, ECornerPosition::TopRight);
    
    // Нижний правый угол
    DrawCornerOutline(OutDrawElements, AllottedGeometry, LayerId, ECornerPosition::BottomRight);
    
    // Нижний левый угол
    DrawCornerOutline(OutDrawElements, AllottedGeometry, LayerId, ECornerPosition::BottomLeft);
}

void SCustomRoundedBox::DrawCornerOutline(
    FSlateWindowElementList& OutDrawElements,
    const FGeometry& AllottedGeometry,
    int32 LayerId,
    ECornerPosition CornerPosition) const
{
    float Radius = 0.0f;
    float BorderWidth = 0.0f;

    switch (CornerPosition)
    {
    case ECornerPosition::TopLeft:
        Radius = Brush.OutlineSettings.CornerRadii.X;
        BorderWidth = Brush.GetTopBorderWidth();
        break;
    case ECornerPosition::TopRight:
        Radius = Brush.OutlineSettings.CornerRadii.Y;
        BorderWidth = Brush.GetTopBorderWidth();
        break;
    case ECornerPosition::BottomRight:
        Radius = Brush.OutlineSettings.CornerRadii.Z;
        BorderWidth = Brush.GetBottomBorderWidth();
        break;
    case ECornerPosition::BottomLeft:
        Radius = Brush.OutlineSettings.CornerRadii.W;
        BorderWidth = Brush.GetBottomBorderWidth();
        break;
    }
    
    if (Radius > 0 && BorderWidth > 0)
    {
        FRoundedCornerElement CornerElement(
            AllottedGeometry,
            Radius,
            BorderWidth,
            Brush.OutlineColor,
            CornerPosition,
            Brush
           );
        
        CornerElement.DrawRoundedCorner(
            AllottedGeometry,
            FSlateRect(FVector2D::ZeroVector, AllottedGeometry.GetLocalSize()),
            OutDrawElements,
            LayerId
        );
    }
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION 